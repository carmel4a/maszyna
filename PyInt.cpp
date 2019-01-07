/*
This Source Code Form is subject to the
terms of the Mozilla Public License, v.
2.0. If a copy of the MPL was not
distributed with this file, You can
obtain one at
http://mozilla.org/MPL/2.0/.
*/

#include "stdafx.h"
#include "PyInt.h"

#include "Globals.h"
#include "application.h"
#include "renderer.h"
#include "Logs.h"
#include "Python/PythonConfig.h"

#ifdef __GNUC__
#pragma GCC diagnostic ignored "-Wwrite-strings"
#endif

using python::is_64_bit;
using python::get_windows_lib_name;
using python::get_linux_lib_name;

void render_task::run() {
    // call the renderer
    auto *output { PyObject_CallMethod( m_renderer, "render", "O", m_input ) };
    Py_DECREF( m_input );

    if( output != nullptr ) {
        auto *outputwidth { PyObject_CallMethod( m_renderer, "get_width", nullptr ) };
        auto *outputheight { PyObject_CallMethod( m_renderer, "get_height", nullptr ) };
        // upload texture data
        if( ( outputwidth != nullptr )
         && ( outputheight != nullptr )
         && m_target != -1) {
			m_width = PyInt_AsLong( outputwidth );
			m_height = PyInt_AsLong( outputheight );

            const unsigned char *image = reinterpret_cast<const unsigned char *>( PyString_AsString( output ) );
			if (!Global.gfx_usegles)
			{
				int size = m_width * m_height * 3;
				m_format = GL_SRGB8;
				m_components = GL_RGB;
				m_image = new unsigned char[size];
				memcpy(m_image, image, size);
			}
			else
			{
				m_format = GL_SRGB8_ALPHA8;
				m_components = GL_RGBA;
				m_image = new unsigned char[m_width * m_height * 4];

				int w = m_width;
				int h = m_height;
				for (int y = 0; y < h; y++)
					for (int x = 0; x < w; x++)
					{
						m_image[(y * w + x) * 4 + 0] = image[(y * w + x) * 3 + 0];
						m_image[(y * w + x) * 4 + 1] = image[(y * w + x) * 3 + 1];
						m_image[(y * w + x) * 4 + 2] = image[(y * w + x) * 3 + 2];
						m_image[(y * w + x) * 4 + 3] = 0xFF;
					}
			}
        }
        if( outputheight != nullptr ) { Py_DECREF( outputheight ); }
        if( outputwidth  != nullptr ) { Py_DECREF( outputwidth ); }
        Py_DECREF( output );
    }
}

void render_task::upload()
{
	if (m_image)
	{
		glBindTexture(GL_TEXTURE_2D, m_target);
		glTexImage2D(
		    GL_TEXTURE_2D, 0,
		    m_format,
		    m_width, m_height, 0,
		    m_components, GL_UNSIGNED_BYTE, m_image);

		delete[] m_image;

		if (Global.python_mipmaps)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		}

		if (Global.python_threadedupload)
			glFlush();
	}

	delete this;
}

void render_task::cancel() {

    Py_DECREF( m_input );
    delete this;
}

bool python_taskqueue::set_io_module()
{
    return true;
}

void python_taskqueue::set_home()
{
    #ifdef _WIN32
        std::string path { python::get_windows_lib_name() };
    #elif __linux__
        std::string path { python::get_linux_lib_name() };
    #endif
    // Set root directory for python, depends on platform.
    Py_SetPythonHome( path.data() );
}

bool python_taskqueue::init()
{
    set_home();

    Py_Initialize();
    PyEval_InitThreads();
    m_initialized = true;

	PyObject* string_io_module     = nullptr,
	        * string_io_class_name = nullptr,
	        * string_io_object     = nullptr;

    // do the setup work while we hold the lock
    m_main = PyImport_ImportModule( "__main__" );
    if( !m_main ) return ( release_and_log_error(
        "Python Interpreter: __main__ module is missing" ), false );

    string_io_module = PyImport_ImportModule( "cStringIO" );
    string_io_class_name = (
        string_io_module != nullptr ?
            PyObject_GetAttrString( string_io_module, "StringIO" ) :
            nullptr );
    string_io_object = (
        string_io_class_name != nullptr ?
            PyObject_CallObject( string_io_class_name, nullptr ) :
            nullptr );
    m_error = { (
        string_io_object == nullptr ? nullptr :
        PySys_SetObject( "stderr", string_io_object ) != 0 ? nullptr :
        string_io_object ) };

    if( m_error == nullptr ) return ( release_and_log_error(
        "Python Interpreter: \\todo" ), false );

    if( false == run_file( "abstractscreenrenderer" ) )
        return ( release(), false );

    // release the lock, save the state for future use
    m_mainthread = PyEval_SaveThread();

    WriteLog( "Python Interpreter setup complete" );

    // init workers
    for( auto &worker : m_workers ) {

		GLFWwindow *openglcontextwindow = nullptr;
		if (Global.python_threadedupload)
			openglcontextwindow = Application.window( -1 );
        worker = std::thread(
                    &python_taskqueue::run, this,
		            openglcontextwindow, std::ref( m_tasks ), std::ref(m_uploadtasks), std::ref( m_condition ), std::ref( m_exit ) );

        if( false == worker.joinable() ) { return false; }
    }

    return true;
}

// shuts down the module
void python_taskqueue::exit() {
    if (!m_initialized)
        return;

    // let the workers know we're done with them
    m_exit = true;
    m_condition.notify_all();
    // let them free up their shit before we proceed
    for( auto &worker : m_workers ) {
        worker.join();
    }
    // get rid of the leftover tasks
    // with the workers dead we don't have to worry about concurrent access anymore
    for( auto *task : m_tasks.data ) {
        task->cancel();
    }
    // take a bow
    acquire_lock();
    Py_Finalize();
}

// adds specified task along with provided collection of data to the work queue. returns true on success
auto python_taskqueue::insert( task_request const &Task ) -> bool {

    if( ( Task.renderer.empty() )
     || ( Task.input == nullptr )
     || ( Task.target == 0 ) ) { return false; }

    auto *renderer { fetch_renderer( Task.renderer ) };
    if( renderer == nullptr ) { return false; }

    auto *newtask { new render_task( renderer, Task.input, Task.target ) };
    bool newtaskinserted { false };
    // acquire a lock on the task queue and add the new task
    {
        std::lock_guard<std::mutex> lock( m_tasks.mutex );
        // check the task list for a pending request with the same target
        for( auto &task : m_tasks.data ) {
            if( task->target() == Task.target ) {
                // replace pending task in the slot with the more recent one
                acquire_lock();
                {
                    task->cancel();
                }
                release_lock();
                task = newtask;
                newtaskinserted = true;
                break;
            }
        }
        if( false == newtaskinserted ) {
            m_tasks.data.emplace_back( newtask );
        }
    }
    // potentially wake a worker to handle the new task
    m_condition.notify_one();
    // all done
    return true;
}

// executes python script stored in specified file. returns true on success
auto python_taskqueue::run_file( std::string const &File, std::string const &Path ) -> bool {

    auto const lookup { FileExists( { Path + File, "python/local/" + File }, { ".py" } ) };
    if( lookup.first.empty() ) { return false; }

    std::ifstream inputfile { lookup.first + lookup.second };
    std::string input;
    input.assign( std::istreambuf_iterator<char>( inputfile ), std::istreambuf_iterator<char>() );

    if( PyRun_SimpleString( input.c_str() ) != 0 ) {
        error();
        return false;
    }

    return true;
}

// acquires the python gil and sets the main thread as current
void python_taskqueue::acquire_lock() {

    PyEval_RestoreThread( m_mainthread );
}

// releases the python gil and swaps the main thread out
void python_taskqueue::release_lock() {

    PyEval_SaveThread();
}

auto python_taskqueue::fetch_renderer( std::string const Renderer ) ->PyObject * {

    auto const lookup { m_renderers.find( Renderer ) };
    if( lookup != std::end( m_renderers ) ) {
        return lookup->second;
    }
    // try to load specified renderer class
    auto const path { substr_path( Renderer ) };
    auto const file { Renderer.substr( path.size() ) };
    PyObject *renderer { nullptr };
    PyObject *rendererarguments { nullptr };
    PyObject *renderername { nullptr };
    acquire_lock();
    {
        if( m_main == nullptr ) {
            ErrorLog( "Python Renderer: __main__ module is missing" );
            goto cache_and_return;
        }

        if( false == run_file( file, path ) ) {
            goto cache_and_return;
        }
        renderername = PyObject_GetAttrString( m_main, file.c_str() );
        if( renderername == nullptr ) {
            ErrorLog( "Python Renderer: class \"" + file + "\" not defined" );
            goto cache_and_return;
        }
        rendererarguments = Py_BuildValue( "(s)", path.c_str() );
        if( rendererarguments == nullptr ) {
            ErrorLog( "Python Renderer: failed to create initialization arguments" );
            goto cache_and_return;
        }
        renderer = PyObject_CallObject( renderername, rendererarguments );

        if( PyErr_Occurred() != nullptr ) {
            error();
            renderer = nullptr;
        }

cache_and_return:
        // clean up after yourself
        if( rendererarguments != nullptr ) {
            Py_DECREF( rendererarguments );
        }
    }
    release_lock();
    // cache the failures as well so we don't try again on subsequent requests
    m_renderers.emplace( Renderer, renderer );
    return renderer;
}

void python_taskqueue::run( GLFWwindow *Context, rendertask_sequence &Tasks, uploadtask_sequence &Upload_Tasks, threading::condition_variable &Condition, std::atomic<bool> &Exit ) {

	if (Context)
		glfwMakeContextCurrent( Context );

    // create a state object for this thread
    PyEval_AcquireLock();
    auto *threadstate { PyThreadState_New( m_mainthread->interp ) };
    PyEval_ReleaseLock();

    render_task *task { nullptr };

    while( false == Exit.load() ) {
        // regardless of the reason we woke up prime the spurious wakeup flag for the next time
        Condition.spurious( true );
        // keep working as long as there's any scheduled tasks
        do {
            task = nullptr;
            // acquire a lock on the task queue and potentially grab a task from it
            {
                std::lock_guard<std::mutex> lock( Tasks.mutex );
                if( false == Tasks.data.empty() ) {
                    // fifo
                    task = Tasks.data.front();
                    Tasks.data.pop_front();
                }
            }
            if( task != nullptr ) {
                // swap in my thread state
                PyEval_RestoreThread( threadstate );
                {
                    // execute python code
                    task->run();
					if (Context)
						task->upload();
					else
					{
						std::lock_guard<std::mutex> lock(Upload_Tasks.mutex);
						Upload_Tasks.data.push_back(task);
					}
                    error();
                }
                // clear the thread state
                PyEval_SaveThread();
            }
            // TBD, TODO: add some idle time between tasks in case we're on a single thread cpu?
        } while( task != nullptr );
        // if there's nothing left to do wait until there is
        // but check every now and then on your own to minimize potential deadlock situations
        Condition.wait_for( std::chrono::seconds( 5 ) );
    }
    // clean up thread state data
    PyEval_AcquireLock();
    PyThreadState_Swap( nullptr );
    PyThreadState_Clear( threadstate );
    PyThreadState_Delete( threadstate );
    PyEval_ReleaseLock();
}

void python_taskqueue::update()
{
	std::lock_guard<std::mutex> lock(m_uploadtasks.mutex);

	for (auto &task : m_uploadtasks.data)
		task->upload();

	m_uploadtasks.data.clear();
}

void python_taskqueue::release_and_log_error( const char* const error )
{
    release();
    ErrorLog( error );
}

void
python_taskqueue::error() {

    if( PyErr_Occurred() == nullptr ) { return; }

    if( m_error != nullptr ) {
        // std err pythona jest buforowane
        PyErr_Print();
        auto *errortext { PyObject_CallMethod( m_error, "getvalue", nullptr ) };
        ErrorLog( PyString_AsString( errortext ) );
        // czyscimy bufor na kolejne bledy
        PyObject_CallMethod( m_error, "truncate", "i", 0 );
    }
    else {
        // nie dziala buffor pythona
        PyObject *type, *value, *traceback;
        PyErr_Fetch( &type, &value, &traceback );
        if( type == nullptr ) {
            ErrorLog( "Python Interpreter: don't know how to handle null exception" );
        }
        PyErr_NormalizeException( &type, &value, &traceback );
        if( type == nullptr ) {
            ErrorLog( "Python Interpreter: don't know how to handle null exception" );
        }
        auto *typetext { PyObject_Str( type ) };
        if( typetext != nullptr ) {
            ErrorLog( PyString_AsString( typetext ) );
        }
        if( value != nullptr ) {
            ErrorLog( PyString_AsString( value ) );
        }
        auto *tracebacktext { PyObject_Str( traceback ) };
        if( tracebacktext != nullptr ) {
            WriteLog( PyString_AsString( tracebacktext ) );
        }
        else {
            WriteLog( "Python Interpreter: failed to retrieve the stack traceback" );
        }
    }
}

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif
