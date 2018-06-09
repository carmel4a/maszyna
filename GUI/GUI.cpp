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
#include "Popup_CA_SHP.h"
#include "PanelPause.h"


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
    nanogui::ref< Theme > default_theme = new DefaultTheme( get_screen()->nvgContext() );
    get_screen()->setTheme( default_theme );
    
    const auto& exit_popup_ref = std::make_shared< PopupExit >();
    add_widget("exit_popup",
                exit_popup_ref,
                widgets
            );
    
    const auto& pause_panel_ref = std::make_shared< PanelPause >();
    add_widget("pause_popup",
                pause_panel_ref,
                widgets
            );
    
    const auto& ca_shp_ref = std::make_shared< Popup_CA_SHP >();
    add_widget("ca_shp",
                ca_shp_ref,
                widgets
            );
    get< Popup_CA_SHP >( "ca_shp" )->show();
    get< PanelPause >( "pause_popup" )->show();
    
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

// call on each frame
void GUI_::draw_gui(){

    if( is_ready ){
        get_screen()->drawContents();
        get_screen()->drawWidgets();
    }
};

[[deprecated]]
void GUI_::focus_helper_on( Window& window ){
    
    GUI.get_helper()->setWindow( &window );
}

void GUI_::add_widget(                 std::string name,
                       const shared_custom_widget& s_ptr_custom_widget,
                                       widget_map& where                ){
    s_ptr_custom_widget->init();
    where[name] = s_ptr_custom_widget;
    s_ptr_custom_widget->make();
    update_layout( s_ptr_custom_widget->widget() );
};

void GUI_::update_layout( Widget* of ){
    
    of->parent()->performLayout( get_screen()->nvgContext() );
};

void GUI_::remove_widget( std::string name,
                          widget_map& from  ){
        auto* parent = from[name].get()->widget()->parent();
        parent->removeChild( from[name].get()->widget() );
        from.erase( name );
        update_layout( parent );
};

void GUI_::update_all_vars(){
    
    helper->refresh();
};

/*
            ,     \    /      ,
           / \    )\__/(     / \   
          /   \  (_\  /_)   /   \                
  _______/_____\__\@  @/___/_____\_______
  |               |\../|                |
  |                \VV/                 |
  |                                     |
  |     Jeśli działa - nie ruszaj       |
  | Jeśli nie działa - sorry            |
  |_____________________________________|
      |    /\ /      \\       \ /\    |
      |  /   V        ))       V   \  |
      |/     `       //        '     \|
      `              V
*/
template< class T >
T* GUI_::get( std::string name ){
            return dynamic_cast< T* >( widgets[name].get() );
};

void GUI_::set_x_anchor( 
        Widget* what,
        Widget* to,
        GUI_::Anchor anchor_x
        ){
    _set_axis_anchor( *what, *to, anchor_x, 0 );
};

void GUI_::set_y_anchor( 
        Widget* what,
        Widget* to,
        GUI_::Anchor anchor_y
        ){
    _set_axis_anchor( *what, *to, anchor_y, 1 );
};

void GUI_::_set_axis_anchor( 
        Widget& what,
        Widget& to,
        GUI_::Anchor anchor,
        short axis ){

    switch( anchor.mode ){
        case GUI_::Alignment::Begin:{

            what.setPosition(
                    _get_rel_to_axis<Vector2i>(
                            ( anchor.is_margin_rel ) ? Vector2i(
                                to.absolutePosition()
                                - what.parent()->absolutePosition()
                                + (anchor.margin_rel * to.size().cast<float>()).cast<int>()
                            ) : Vector2i(
                                to.absolutePosition() 
                                - what.parent()->absolutePosition()
                                + Vector2i( anchor.margin, anchor.margin )
                            ),
                            what.absolutePosition(),
                            axis )
            );
            update_layout( &what );
            break;
        }
        case GUI_::Alignment::Centered:{
            what.setPosition(
                    _get_rel_to_axis<Vector2i>(
                            to.absolutePosition() 
                            - what.parent()->absolutePosition() 
                            + ( 0.5F * to.size().cast<float>() ).cast<int>()
                            - ( 0.5F * what.size().cast<float>() ).cast<int>(),
                            what.absolutePosition(),
                            axis )
            );
            update_layout( &what );
            break;
        }
        case GUI_::Alignment::End:{

            what.setPosition(
                    _get_rel_to_axis<Vector2i>(
                            ( anchor.is_margin_rel ) ? Vector2i(
                                to.absolutePosition()
                                - what.parent()->absolutePosition()
                                + to.size()
                                - what.size()
                                - (anchor.margin_rel * to.size().cast<float>()).cast<int>()
                            ) : Vector2i(
                                to.absolutePosition() 
                                - what.parent()->absolutePosition()
                                + to.size()
                                - what.size()
                                - Vector2i( anchor.margin, anchor.margin )
                            ),
                            what.absolutePosition(),
                            axis )
            );
            update_layout( &what );
            break;
        }
        case GUI_::Alignment::Fill:{
            what.setPosition(
                    _get_rel_to_axis<Vector2i>( to.absolutePosition() - what.parent()->absolutePosition(),
                            what.absolutePosition(),
                            axis )
            );
            what.setFixedSize( 
                    _get_rel_to_axis( to.size(),
                            what.size(),
                            axis )
            );
            update_layout( &what );
            break;
        }
    }
};

InputScreen::InputScreen(){
    setResizeCallback(
        [this]( Vector2i v )->void{
            resize( v ); 
        }
    );
    //setResizeCallback( std::bind( &InputScreen::resize(), this, std::placeholders::_1 ) );
};

void InputScreen::resize( nanogui::Vector2i v ){
    for( auto const& [key, val] : GUI.widgets )
    {
        val->resize( v );
    }
};

bool InputScreen::keyboardEvent(int key, int scancode, int action, int modifiers){
    
    if( key == GLFW_KEY_F10 ){
        const auto& popup = dynamic_cast< PopupExit* >( GUI.widgets["exit_popup"].get() );
        if( popup->may_quit && action == GLFW_PRESS ){
            if( popup->widget()->visible() ){
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

#define FONT_SCALE 2
DefaultTheme::DefaultTheme( NVGcontext *ctx ):
    Theme( ctx ){
    mStandardFontSize = 16 * FONT_SCALE;
    mButtonFontSize = 20 * FONT_SCALE;
    mTextBoxFontSize = 20 * FONT_SCALE;
};

DefaultTheme::~DefaultTheme(){};
