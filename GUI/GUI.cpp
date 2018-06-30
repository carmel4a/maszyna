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
#include "FSM.h"
#include "Logs.h"
#include "Globals.h"
#include "CustomWidget.h"
#include "PopupExit.h"
#include "Popup_CA_SHP.h"
#include "PanelPause.h"
#include "LabelArray.h"

#include "Yoga.h"


using namespace nanogui;
class InputScreen; class PopupExit; class LabelArray; class LoadingLog;
struct GUI_FSM; struct On; struct PrintLine;

GUI_ GUI;

GUI_::GUI_(){

    screen_ = std::make_shared< InputScreen >();
    // Create a gui helper. Creating windows, widgets, etc.
    helper = std::make_shared< FormHelper >( screen() );
    WriteLog( "GUI created." );
};

GUI_::~GUI_(){

    nanogui::shutdown();
    WriteLog("GUI deleted.");
};

//getters. .get() should return raw pointer.
Screen* GUI_::get_screen(){

    return screen.get();
};

// call on each frame
void GUI_::draw_gui(){

    if( may_render ){
        update_vars();
        get_screen()->drawContents();
        get_screen()->drawWidgets();
    }
};

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

void GUI_::update_vars(){
    for( auto const& x : widgets ){
        if( x.second->may_update ){
            x.second->update();
        }
    }
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
                                + Vector2i( -anchor.margin, -anchor.margin )
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

void GUI_::Start::react( GUI_Init const & ) {

    transit<GUI_::LoadingScreen>();
};
void GUI_::Start::entry() { };
void GUI_::Start::exit() {

    nanogui::ref< Theme > default_theme = new DefaultTheme( GUI.screen()->nvgContext() );
    GUI.screen()->setTheme( default_theme );

    //const auto& loading_log_ref = std::make_shared< LoadingLog >();
    GUI.screen()->setVisible( true );
    // Mark gui as ready to be drawn.
    GUI.may_render = true;

    WriteLog( "GUI prepared." );
};

void GUI_::LoadingScreen::react( PrintLine const & e ) { 
    if( Global.loading_log ){
        GUI.get<LabelArray>("loading_log")->push_line( e.text );
    }
 };
void GUI_::LoadingScreen::react( SceneLoaded const & ) { transit<GUI_::Simulation>(); };
void GUI_::LoadingScreen::entry() { 
    
    if( Global.loading_log ){
        const auto& log_widget_ref2 = std::make_shared< LabelArray >(
            false,     // transparent = true,
            "Log",     //std::string Name = "Label Array",
            15,        // Size = 10,
            -1         // int fixed_w = -1,
                       //std::string Def_text = ""
        );
        GUI.add_widget("loading_log",
                        log_widget_ref2,
                        GUI.widgets
        );
    }
 };

void GUI_::LoadingScreen::exit(){

    GUI.widgets["loading_log"]->hide();
}; 

//void GUI_::LoadingScreen::react( SceneLoaded const & ) { transit<Simulation>(); };
void GUI_::Simulation::entry() {
    WriteLog("SimulationState.");

    const auto& exit_popup_ref = std::make_shared< PopupExit >();
    GUI.add_widget("exit_popup",
                    exit_popup_ref,
                    GUI.widgets
    );
    
    const auto& pause_panel_ref = std::make_shared< PanelPause >();
    GUI.add_widget("pause_panel",
                    pause_panel_ref,
                    GUI.widgets
    );
    
    const auto& ca_shp_ref = std::make_shared< Popup_CA_SHP >();
    GUI.add_widget("ca_shp",
                    ca_shp_ref,
                    GUI.widgets
    );
    /*
    const auto& log_widget_ref = std::make_shared< LabelArray >(
            true,     // transparent = true,
            "opcja 1", //std::string Name = "Label Array",
            10,        // Size = 10,
            500        // int fixed_w = -1,
                       //std::string Def_text = ""
    );
    add_widget("log_widget",
                log_widget_ref,
                widgets
    );
    */
    const auto& log_widget_ref2 = std::make_shared< LabelArray >(
            false,     // transparent = true,
            "opcja 1", //std::string Name = "Label Array",
            10,        // Size = 10,
            -1        // int fixed_w = -1,
                       //std::string Def_text = ""
    );
    GUI.add_widget("log_widget2",
                    log_widget_ref2,
                    GUI.widgets
    );
};

FSM_INITIAL_STATE(GUI_::GUI_FSM, GUI_::Start)

