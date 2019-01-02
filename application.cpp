﻿/*
This Source Code Form is subject to the
terms of the Mozilla Public License, v.
2.0. If a copy of the MPL was not
distributed with this file, You can
obtain one at
http://mozilla.org/MPL/2.0/.
*/

#include "stdafx.h"
#include "application.h"
#include "scenarioloadermode.h"
#include "drivermode.h"
#include "editormode.h"

#include "Globals.h"
#include "simulation.h"
#include "Train.h"
#include "sceneeditor.h"
#include "renderer.h"
#include "uilayer.h"
#include "Logs.h"
#include "screenshot.h"
#include "translation.h"
#include "Train.h"
#include "Timer.h"

#pragma comment (lib, "glu32.lib")
#pragma comment (lib, "dsound.lib")
#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "setupapi.lib")
#pragma comment (lib, "dbghelp.lib")
#pragma comment (lib, "version.lib")

#ifdef __linux__
#include <unistd.h>
#include <sys/stat.h>
#endif

eu07_application Application;
screenshot_manager screenshot_man;

ui_layer uilayerstaticinitializer;

#ifdef _WIN32
extern "C"
{
    GLFWAPI HWND glfwGetWin32Window( GLFWwindow* window );
}

LONG CALLBACK unhandled_handler( ::EXCEPTION_POINTERS* e );
LRESULT APIENTRY WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
extern HWND Hwnd;
extern WNDPROC BaseWindowProc;
#endif

// user input callbacks

void focus_callback( GLFWwindow *window, int focus ) {
    if( Global.bInactivePause ) {// jeśli ma być pauzowanie okna w tle
        if( focus )
            Global.iPause &= ~4; // odpauzowanie, gdy jest na pierwszym planie
        else
            Global.iPause |= 4; // włączenie pauzy, gdy nieaktywy
    }
}

void window_resize_callback( GLFWwindow *window, int w, int h ) {
    // NOTE: we have two variables which basically do the same thing as we don't have dynamic fullscreen toggle
    // TBD, TODO: merge them?
    Global.iWindowWidth = w;
    Global.iWindowHeight = h;
    Global.fDistanceFactor = std::max( 0.5f, h / 768.0f ); // not sure if this is really something we want to use
    glViewport( 0, 0, w, h );
}

void cursor_pos_callback( GLFWwindow *window, double x, double y ) {
    Application.on_cursor_pos( x, y );
}

void mouse_button_callback( GLFWwindow* window, int button, int action, int mods )
{
    Application.on_mouse_button( button, action, mods );
}

void scroll_callback( GLFWwindow* window, double xoffset, double yoffset ) {
    Application.on_scroll( xoffset, yoffset );
}

void key_callback( GLFWwindow *window, int key, int scancode, int action, int mods ) {

    Application.on_key( key, scancode, action, mods );
}

void char_callback(GLFWwindow *window, unsigned int c)
{
    Application.on_char(c);
}

// public:

void eu07_application::queue_screenshot()
{
    m_screenshot_queued = true;
}

int
eu07_application::init( int Argc, char *Argv[] ) {

    int result { 0 };

    init_debug();
    init_files();
    if( ( result = init_settings( Argc, Argv ) ) != 0 ) {
        return result;
    }
    if( ( result = init_locale() ) != 0 ) {
        return result;
    }

    WriteLog( "Starting MaSzyna rail vehicle simulator (release: " + Global.asVersion + ")" );
    WriteLog( "For online documentation and additional files refer to: http://eu07.pl" );
    WriteLog( "Authors: Marcin_EU, McZapkie, ABu, Winger, Tolaris, nbmx, OLO_EU, Bart, Quark-t, "
        "ShaXbee, Oli_EU, youBy, KURS90, Ra, hunter, szociu, Stele, Q, firleju and others\n" );

    if( ( result = init_glfw() ) != 0 ) {
        return result;
    }
    if( ( result = init_gfx() ) != 0 ) {
        return result;
    }
    init_callbacks();
    if( ( result = init_audio() ) != 0 ) {
        return result;
    }
    m_taskqueue.init();
    if( ( result = init_modes() ) != 0 ) {
        return result;
    }

	if (!init_network())
		return -1;

    return result;
}

void eu07_application::request_train(std::string name) {
	m_network->request_train(name);
}

void eu07_application::spawn_train(std::string name) {
	TTrain *train = simulation::Trains.find(name);
	if (train)
		return;

	TDynamicObject *dynobj = simulation::Vehicles.find(name);
	if (!dynobj)
		return;

	train = new TTrain();
	if (train->Init(dynobj)) {
		simulation::Trains.insert(train, name);
	}
	else {
		delete train;
		train = nullptr;
	}
}

int
eu07_application::run() {

    // main application loop
    while (!glfwWindowShouldClose( m_windows.front() ) && !m_modestack.empty())
    {
        Timer::subsystem.mainloop_total.start();
        glfwPollEvents();

		if (!m_network)
		{
			if (!m_modes[ m_modestack.top() ]->update())
				break;
		}
		else if (!Global.network_conf.is_server) {
			command_queue_client *queue = dynamic_cast<command_queue_client*>(simulation::Commands.get());
			double delta;
			do {
			   auto tup = m_network->get_next_delta();
			   delta = std::get<0>(tup);
			   queue->push_server_commands(std::get<1>(tup));
			   m_network->send_commands(queue->pop_queued_commands());
			   Timer::set_delta_override(delta);

				if (!m_modes[ m_modestack.top() ]->update())
					break;
			}
			while (delta != 0.0);
		}
		else {
			command_queue_server *queue = dynamic_cast<command_queue_server*>(simulation::Commands.get());
			auto commands = queue->pop_queued_commands();

			queue->push_client_commands(m_network->pop_commands());
			queue->update();
			if (!m_modes[ m_modestack.top() ]->update())
				break;

			double delta = Timer::GetDeltaTime();
			m_network->push_delta(delta, commands);
		}

		m_taskqueue.update();
		opengl_texture::reset_unit_cache();

        if (!GfxRenderer.Render())
            break;

        GfxRenderer.SwapBuffers();

        if (m_modestack.empty())
            return 0;

        m_modes[ m_modestack.top() ]->on_event_poll();

        if (m_screenshot_queued) {
            m_screenshot_queued = false;
            screenshot_man.make_screenshot();
        }

        Timer::subsystem.mainloop_total.stop();

		if (m_network)
			m_network->poll();
    }

    return 0;
}

// issues request for a worker thread to perform specified task. returns: true if task was scheduled
bool
eu07_application::request( python_taskqueue::task_request const &Task ) {

    return m_taskqueue.insert( Task );
}

// ensures the main thread holds the python gil and can safely execute python calls
void
eu07_application::acquire_python_lock() {

    m_taskqueue.acquire_lock();
}

// frees the python gil and swaps out the main thread
void
eu07_application::release_python_lock() {

    m_taskqueue.release_lock();
}

void
eu07_application::exit() {

    SafeDelete( simulation::Train );
    SafeDelete( simulation::Region );

    ui_layer::shutdown();

    for( auto *window : m_windows ) {
        glfwDestroyWindow( window );
    }
    m_taskqueue.exit();
    glfwTerminate();
}

void
eu07_application::render_ui() {

    if( m_modestack.empty() ) { return; }

    m_modes[ m_modestack.top() ]->render_ui();
}

bool
eu07_application::pop_mode() {

    if( m_modestack.empty() ) { return false; }

    m_modes[ m_modestack.top() ]->exit();
    m_modestack.pop();
    return true;
}

bool
eu07_application::push_mode( eu07_application::mode const Mode ) {

    if( Mode >= mode::count_ ) { return false; }

    m_modes[ Mode ]->enter();
    m_modestack.push( Mode );

    return true;
}

void
eu07_application::set_title( std::string const &Title ) {

    glfwSetWindowTitle( m_windows.front(), Title.c_str() );
}

void
eu07_application::set_progress( float const Progress, float const Subtaskprogress ) {

    if( m_modestack.empty() ) { return; }

    m_modes[ m_modestack.top() ]->set_progress( Progress, Subtaskprogress );
}

void
eu07_application::set_cursor( int const Mode ) {

    ui_layer::set_cursor( Mode );
}

void
eu07_application::set_cursor_pos( double const Horizontal, double const Vertical ) {

    glfwSetCursorPos( m_windows.front(), Horizontal, Vertical );
}

glm::dvec2 eu07_application::get_cursor_pos() const {
    glm::dvec2 pos;
    if( !m_windows.empty() ) {
        glfwGetCursorPos( m_windows.front(), &pos.x, &pos.y );
    }
    return pos;
}

void
eu07_application::get_cursor_pos( double &Horizontal, double &Vertical ) const {

    glfwGetCursorPos( m_windows.front(), &Horizontal, &Vertical );
}

void
eu07_application::on_key( int const Key, int const Scancode, int const Action, int const Mods ) {

    if (ui_layer::key_callback(Key, Scancode, Action, Mods))
        return;

    if( m_modestack.empty() ) { return; }

    m_modes[ m_modestack.top() ]->on_key( Key, Scancode, Action, Mods );
}

void
eu07_application::on_cursor_pos( double const Horizontal, double const Vertical ) {

    if( m_modestack.empty() ) { return; }

    m_modes[ m_modestack.top() ]->on_cursor_pos( Horizontal, Vertical );
}

void
eu07_application::on_mouse_button( int const Button, int const Action, int const Mods ) {

    if (ui_layer::mouse_button_callback(Button, Action, Mods))
        return;

    if( m_modestack.empty() ) { return; }

    m_modes[ m_modestack.top() ]->on_mouse_button( Button, Action, Mods );
}

void
eu07_application::on_scroll( double const Xoffset, double const Yoffset ) {

    if (ui_layer::scroll_callback(Xoffset, Yoffset))
        return;

    if( m_modestack.empty() ) { return; }

    m_modes[ m_modestack.top() ]->on_scroll( Xoffset, Yoffset );
}

void eu07_application::on_char(unsigned int c) {
    if (ui_layer::char_callback(c))
        return;
}

GLFWwindow *
eu07_application::window( int const Windowindex ) {

    if( Windowindex >= 0 ) {
        return (
            Windowindex < m_windows.size() ?
                m_windows[ Windowindex ] :
                nullptr );
    }
    // for index -1 create a new child window
    glfwWindowHint( GLFW_VISIBLE, GL_FALSE );
    auto *childwindow = glfwCreateWindow( 1, 1, "eu07helper", nullptr, m_windows.front() );
    if( childwindow != nullptr ) {
        m_windows.emplace_back( childwindow );
    }
    return childwindow;
}

// private:

void
eu07_application::init_debug() {

#if defined(_MSC_VER) && defined (_DEBUG)
    // memory leaks
    _CrtSetDbgFlag( _CrtSetDbgFlag( _CRTDBG_REPORT_FLAG ) | _CRTDBG_LEAK_CHECK_DF );
    // floating point operation errors
    auto state { _clearfp() };
    state = _control87( 0, 0 );
    // this will turn on FPE for #IND and zerodiv
    state = _control87( state & ~( _EM_ZERODIVIDE | _EM_INVALID ), _MCW_EM );
#endif
#ifdef _WIN32
    ::SetUnhandledExceptionFilter( unhandled_handler );
#endif
}

void
eu07_application::init_files() {

#ifdef _WIN32
    DeleteFile( "log.txt" );
    DeleteFile( "errors.txt" );
    CreateDirectory( "logs", NULL );
#elif __linux__
	unlink("log.txt");
	unlink("errors.txt");
	mkdir("logs", 0664);
#endif
}

int
eu07_application::init_settings( int Argc, char *Argv[] ) {

    Global.LoadIniFile( "eu07.ini" );
#ifdef _WIN32
    if( ( Global.iWriteLogEnabled & 2 ) != 0 ) {
        // show output console if requested
        AllocConsole();
    }
#endif

	Global.asVersion = VERSION_INFO;

    // process command line arguments
    for( int i = 1; i < Argc; ++i ) {

        std::string token { Argv[ i ] };

        if( token == "-s" ) {
            if( i + 1 < Argc ) {
                Global.SceneryFile = ToLower( Argv[ ++i ] );
            }
        }
        else if( token == "-v" ) {
            if( i + 1 < Argc ) {
                Global.asHumanCtrlVehicle = ToLower( Argv[ ++i ] );
            }
        }
        else {
            std::cout
                << "usage: " << std::string( Argv[ 0 ] )
                << " [-s sceneryfilepath]"
                << " [-v vehiclename]"
                << std::endl;
            return -1;
        }
    }

    return 0;
}

int
eu07_application::init_locale() {

    locale::init();

    return 0;
}

int
eu07_application::init_glfw() {

    if( glfwInit() == GLFW_FALSE ) {
        ErrorLog( "Bad init: failed to initialize glfw" );
        return -1;
    }

    // match requested video mode to current to allow for
    // fullwindow creation when resolution is the same
    auto *monitor { glfwGetPrimaryMonitor() };
    auto const *vmode { glfwGetVideoMode( monitor ) };

    glfwWindowHint( GLFW_RED_BITS, vmode->redBits );
    glfwWindowHint( GLFW_GREEN_BITS, vmode->greenBits );
    glfwWindowHint( GLFW_BLUE_BITS, vmode->blueBits );
    glfwWindowHint( GLFW_REFRESH_RATE, vmode->refreshRate );

    glfwWindowHint(GLFW_SRGB_CAPABLE, !Global.gfx_shadergamma);

    if (!Global.gfx_usegles)
    {
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    }
    else
    {
        glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    }

    glfwWindowHint( GLFW_AUTO_ICONIFY, GLFW_FALSE );
    if (Global.gfx_skippipeline && Global.iMultisampling > 0) {
        glfwWindowHint( GLFW_SAMPLES, 1 << Global.iMultisampling );
    }

    auto *window {
        glfwCreateWindow(
            Global.iWindowWidth,
            Global.iWindowHeight,
            Global.AppName.c_str(),
            ( Global.bFullScreen ?
                monitor :
                nullptr ),
            nullptr ) };

    if( window == nullptr ) {
        ErrorLog( "Bad init: failed to create glfw window" );
        return -1;
    }

    glfwMakeContextCurrent( window );
    glfwSwapInterval( Global.VSync ? 1 : 0 ); //vsync

#ifdef _WIN32
// setup wrapper for base glfw window proc, to handle copydata messages
    Hwnd = glfwGetWin32Window( window );
    BaseWindowProc = ( WNDPROC )::SetWindowLongPtr( Hwnd, GWLP_WNDPROC, (LONG_PTR)WndProc );
    // switch off the topmost flag
    ::SetWindowPos( Hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
#endif

	if (Global.captureonstart)
	{
		Global.ControlPicking = false;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	else
		Global.ControlPicking = true;

    m_windows.emplace_back( window );

    return 0;
}

void
eu07_application::init_callbacks() {

    auto *window { m_windows.front() };
    glfwSetFramebufferSizeCallback( window, window_resize_callback );
    glfwSetCursorPosCallback( window, cursor_pos_callback );
    glfwSetMouseButtonCallback( window, mouse_button_callback );
    glfwSetKeyCallback( window, key_callback );
    glfwSetScrollCallback( window, scroll_callback );
    glfwSetCharCallback(window, char_callback);
    glfwSetWindowFocusCallback( window, focus_callback );
    {
        int width, height;
        glfwGetFramebufferSize( window, &width, &height );
        window_resize_callback( window, width, height );
    }
}

int
eu07_application::init_gfx() {

    if (!Global.gfx_usegles)
    {
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            ErrorLog( "Bad init: failed to initialize glad" );
            return -1;
        }
    }
    else
    {
        if (!gladLoadGLES2Loader((GLADloadproc)glfwGetProcAddress))
        {
            ErrorLog( "Bad init: failed to initialize glad" );
            return -1;
        }
    }

    if (!ui_layer::init(m_windows.front()))
        return -1;

    if (!GfxRenderer.Init(m_windows.front()))
        return -1;

    return 0;
}

int
eu07_application::init_audio() {

    if( Global.bSoundEnabled ) {
        Global.bSoundEnabled &= audio::renderer.init();
    }
    // NOTE: lack of audio isn't deemed a failure serious enough to throw in the towel
    return 0;
}

int
eu07_application::init_modes() {

    // NOTE: we could delay creation/initialization until transition to specific mode is requested,
    // but doing it in one go at the start saves us some error checking headache down the road

    // create all application behaviour modes
    m_modes[ mode::scenarioloader ] = std::make_shared<scenarioloader_mode>();
    m_modes[ mode::driver ] = std::make_shared<driver_mode>();
    m_modes[ mode::editor ] = std::make_shared<editor_mode>();
    // initialize the mode objects
    for( auto &mode : m_modes ) {
        if( false == mode->init() ) {
            return -1;
        }
    }
    // activate the default mode
    push_mode( mode::scenarioloader );

    return 0;
}

bool eu07_application::init_network() {
	if (Global.network_conf.enabled) {
		m_network.emplace();
		if (Global.network_conf.is_server) {
			m_network->create_server();
			simulation::Commands = std::make_unique<command_queue_server>();
		}
		else {
			m_network->connect();
			simulation::Commands = std::make_unique<command_queue_client>();
		}
	}
	else
		simulation::Commands = std::make_unique<command_queue>();

	return true;
}
