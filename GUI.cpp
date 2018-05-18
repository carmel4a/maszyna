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
#include "PopupExit.h"

using namespace nanogui;
class PopupExit;

GUI_ GUI;

GUI_::GUI_(){

    screen = std::make_shared< InputScreen >();
    // Create a gui helper. Creating windows, widgets, etc.
    helper = std::make_shared< FormHelper >( get_screen() );
    WriteLog( "GUI created." );
};

GUI_::~GUI_(){

    nanogui::shutdown();
    WriteLog("GUI deleted.");
};

// Init must be called AFTER set callbacks in EU07.cpp
void GUI_::init( GLFWwindow* window ){

    get_screen()->initialize( window, true );
    
    const auto& exit_popup_ref = std::make_shared< PopupExit >();
    add_widget("exit_popup",
                exit_popup_ref,
                widgets
            );
    

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
}

void GUI_::add_widget(                 std::string name,
                       const shared_custom_widget& s_ptr_custom_widget,
                                       widget_map& where                ){
    s_ptr_custom_widget->init();
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

bool InputScreen::keyboardEvent(int key, int scancode, int action, int modifiers){
    
    if( key == GLFW_KEY_F10 ){
        const auto& popup = dynamic_cast< PopupExit* >( GUI.widgets["exit_popup"].get() );
        if( popup->may_quit && action == GLFW_PRESS ){
            if( popup->get_widget()->visible() ){
                popup->hide();
            }
            else{
                popup->show();
            }
            return true;
        }
    }

    if (mFocusPath.size() > 0) {
        for (auto it = mFocusPath.rbegin() + 1; it != mFocusPath.rend(); ++it)
            if ((*it)->focused() && (*it)->keyboardEvent(key, scancode, action, modifiers))
                return true;
    }
    
    return false;
};

/*
Vector2i to_glob_pos(Vector2f rel_pos, Vector2i start_pos=Eigen::Vector2i(), Vector2i end_pos){

};
Vector2f to_rel_pos(Vector2f glob_pos){};
*/
