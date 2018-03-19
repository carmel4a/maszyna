/*
    This Source Code Form is subject to the
    terms of the Mozilla Public License, v.
    2.0. If a copy of the MPL was not
    distributed with this file, You can
    obtain one at
    http://mozilla.org/MPL/2.0/.
*/

#include <memory>
#include <vector>
#include <string>
#include <typeinfo>

#include "GUI.h"
#include "Logs.h"
#include "Globals.h"
#include "CustomWidget.h"

using namespace nanogui;

GUI_ GUI;

GUI_::GUI_(){

    screen = std::make_shared< Screen >();
    // Create a gui helper. Creating windows, widgets, etc.
    helper = std::make_shared< FormHelper >( get_screen() );
    WriteLog( "GUI created." );
};

GUI_::~GUI_(){

    WriteLog("GUI deleted.");
};

// Init must be called AFTER set callbacks in EU07.cpp
void GUI_::init( GLFWwindow* window ){

    get_screen()->initialize( window, true );
    get_screen()->setLayout( new BoxLayout( Orientation::Vertical ) );

    const shared_custom_widget& _ref =
            std::make_shared< TestWidget >( "llalla", get_screen() );
    add_widget("a",
             _ref,
              widgets
             );
    
    const shared_custom_widget& _ref1 =
            std::make_shared< TestWidget >( "test2", widgets["a"]->get_widget_ref() );
    add_widget("b",
              _ref1,
               widgets
             );
    
    const shared_custom_widget& _ref2 =
            std::make_shared< TestWidget >( "test2", widgets["a"]->get_widget_ref() );
    add_widget("c",
        _ref2,
        widgets
             );
    remove_widget( "b", widgets );
    get_screen()->setVisible( true );
    // Mark gui as ready to be drawn.
    is_ready = true;
    WriteLog( "GUI prepared." );
};

//getters. .get() should return raw pointer.
Screen* GUI_::get_screen(){

    return screen.get();
};

FormHelper* GUI_::get_helper(){

    return helper.get();
};

void GUI_::draw_gui(){

    if( is_ready ){
        get_screen()->drawContents();
        get_screen()->drawWidgets();
    }
};

void GUI_::focus_helper_on( Window& window ){
    
    GUI.get_helper()->setWindow( &window );
};

void GUI_::add_widget(                 std::string name,
                       const shared_custom_widget& s_ptr_custom_widget,
                                       widget_map& where                ){
    where[name] = s_ptr_custom_widget;
    s_ptr_custom_widget->init_layout();
    s_ptr_custom_widget->make();
    update_layout( s_ptr_custom_widget->get_widget() );
};

void GUI_::update_layout( Widget* of ){

    of->performLayout( get_screen()->nvgContext() );
    of->parent()->performLayout( get_screen()->nvgContext() );
    get_screen()->performLayout( get_screen()->nvgContext() );
}

void GUI_::remove_widget( std::string name,
                          widget_map& from  ){
        auto* parent = from[name].get()->get_widget()->parent();
        parent->removeChild( from[name].get()->get_widget() );
        from.erase( name );
        update_layout( parent );
};

void GUI_::update_all_vars(){
    
    helper->refresh();
};

/*
Vector2i to_glob_pos(Vector2f rel_pos, Vector2i start_pos=Eigen::Vector2i(), Vector2i end_pos){

};
Vector2f to_rel_pos(Vector2f glob_pos){};
*/
