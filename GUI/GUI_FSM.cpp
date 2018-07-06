/*
    This Source Code Form is subject to the
    terms of the Mozilla Public License, v.
    2.0. If a copy of the MPL was not
    distributed with this file, You can
    obtain one at
    http://mozilla.org/MPL/2.0/.
*/

#include "GUI_FSM.h"
#include "GUI.h"
#include "FSM.h"
#include "tinyfsm.hpp"

#include "LabelArray.h"
#include "PopupExit.h"
#include "PanelPause.h"
#include "Popup_CA_SHP.h"

#include "Globals.h"
#include "UI_Simulation.h"
#include "UI_LoadingScreen.h"

////////////
// States //
////////////

///////////
// Start //
///////////

void GUI_States::Start::entry(){

};

void GUI_States::Start::exit(){

    nanogui::ref< Theme > default_theme = new DefaultTheme( GUI.screen()->nvgContext() );

    GUI.screen()->setTheme( default_theme );
    GUI.screen()->setVisible( true );
    dynamic_cast<InputScreen*>(GUI.screen())->resize( Vector2i( Global.iWindowWidth, Global.iWindowHeight ) );
    GUI.set_ready( true );

    WriteLog( "GUI prepared." );
};

void GUI_States::Start::react( GUI_Events::Init const & ){

    transit< GUI_States::LoadingScreen >();
};

///////////////////
// LoadingScreen //
///////////////////

void GUI_States::LoadingScreen::entry(){ 

    const auto& loading_screen_ui = std::make_shared< UI_LoadingScreen >();
    GUI.set_root( loading_screen_ui );
    
    if( Global.loading_log ){
        const auto& log_temp = std::make_shared< LabelArray >(
            false,     // transparent = true,
            "Log",     //std::string Name = "Label Array",
            15,        // Size = 10,
            -1         // int fixed_w = -1,
                       //std::string Def_text = ""
        );
        GUI.add_widget("loading_log", log_temp, GUI.root->widgets );
    }
};

void GUI_States::LoadingScreen::exit(){
    //GUI.remove_from_layout( GUI.root->widgets["loading_log"]->YG_node, dynamic_cast< UI_LoadingScreen* >( GUI.root.get() )->top );
    /*
    if( Global.loading_log ){
        GUI.remove_widget(
            "loading_log",
            GUI.root->widgets
        );
    }
    */
    // GUI.widgets["temp_loading_log"] = GUI.widgets["loading_log"];
    if( Global.loading_log ){
        GUI.root->widgets["loading_log"]->hide();
    }
}; 

void GUI_States::LoadingScreen::react( PrintLine const & e ){

    if( Global.loading_log ){
        if ( GUI.root->widgets.find("loading_log") == GUI.root->widgets.end() )
            return;
        //GUI.get< LabelArray >("loading_log")->push_line( (std::string)e.text );
        dynamic_cast< LabelArray* >( GUI.root->widgets["loading_log"].get() )
                ->push_line( (std::string)e.text );
    }
};

void GUI_States::LoadingScreen::react( GUI_Events::SceneLoaded const & ){
    
    transit< GUI_States::Simulation >();
};

////////////////
// Simulation //
////////////////

void GUI_States::Simulation::entry(){

    const auto& simulation_ui = std::make_shared< UI_Simulation >();

    GUI.set_root( simulation_ui );

    const auto& exit_popup = std::make_shared< PopupExit >();
    GUI.add_widget("exit_popup", exit_popup, GUI.root->widgets );
    
    const auto& pause_panel = std::make_shared< PanelPause >();
    GUI.add_widget("pause_panel", pause_panel, GUI.root->widgets );

    const auto& ca_shp = std::make_shared< Popup_CA_SHP >();
    GUI.add_widget("ca_shp", ca_shp, GUI.root->widgets );

    const auto& ca_shp1 = std::make_shared< Popup_CA_SHP >();
    GUI.add_widget("ca_shp1", ca_shp1, GUI.root->widgets );

    const auto& ca_shp2 = std::make_shared< Popup_CA_SHP >();
    GUI.add_widget("ca_shp2", ca_shp2, GUI.root->widgets );

/*
 const auto& log_temp2 = std::make_shared< LabelArray >(
            false,     // transparent = true,
            "Log",     //std::string Name = "Label Array",
            15,        // Size = 10,
            -1         // int fixed_w = -1,
                       //std::string Def_text = ""
        );*/
    //GUI.add_widget("loading_log2", log_temp2, GUI.root->widgets, false );
    //GUI.add_to_layout( GUI.root->widgets["loading_log2"]->YG_node, dynamic_cast< UI_Simulation* >( GUI.root.get() )->right );
};

void GUI_States::Simulation::exit(){

    GUI.remove_widget( "pause_panel", GUI.root->widgets );

    GUI.remove_widget( "ca_shp", GUI.root->widgets );
};

FSM_INITIAL_STATE( GUI_FSM, GUI_States::Start )
