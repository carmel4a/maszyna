/*
    This Source Code Form is subject to the
    terms of the Mozilla Public License, v.
    2.0. If a copy of the MPL was not
    distributed with this file, You can
    obtain one at
    http://mozilla.org/MPL/2.0/.
*/
#include <memory>
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

    std::shared_ptr<UI_LoadingScreen> loading_screen_ui( new UI_LoadingScreen() );
    loading_screen_ui->init();
    GUI.set_root( loading_screen_ui );
    
    if( Global.loading_log ){
        std::shared_ptr<LabelArray> _log_temp( new LabelArray( 
            "loading_log",
            GUI.root,
            false,     // transparent = true,
            "Log",     //std::string Name = "Label Array",
            15,        // Size = 10,
            -1         // int fixed_w = -1,
                       //std::string Def_text = ""
        ) );
        _log_temp->init();
    }
};

void GUI_States::LoadingScreen::exit(){
    
    if( Global.loading_log ){
        using LabelArrayData = LabelArray::Data;
        LabelArray * _tmp_ptr = dynamic_cast<LabelArray*>(
                GUI.root->get_child( "loading_log" )
        );
        GUI.memory["loading_log"] = \
                std::shared_ptr<LabelArrayData>(
                    new LabelArrayData(_tmp_ptr->save())
                );
    }
};

void GUI_States::LoadingScreen::react( PrintLine const & e ){

    if( Global.loading_log && GUI.root && GUI.root->get_child("loading_log") ){
        //GUI.get< LabelArray >("loading_log")->push_line( (std::string)e.text );
        dynamic_cast< LabelArray* >( GUI.root->get_child("loading_log") )
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

    std::shared_ptr<UI_Simulation> simulation_ui( new UI_Simulation() );
    simulation_ui->init();
    GUI.set_root( simulation_ui );
    if( Global.loading_log ){

        std::shared_ptr<LabelArray> _log_temp( new LabelArray( 
            "loading_log",
            GUI.root,
            false,     // transparent = true,
            "Log",     //std::string Name = "Label Array",
            15,        // Size = 10,
            -1         // int fixed_w = -1,
                    //std::string Def_text = ""
        ) );
        _log_temp->init();
        dynamic_cast<LabelArray*>( GUI.root->get_child( "loading_log" ) )
        ->load(
            dynamic_cast<LabelArray::Data*>(
                GUI.memory["loading_log"].get()
            )
        );
        GUI.memory.erase("loading_log");
    }


    std::shared_ptr<PopupExit> exit_popup( new PopupExit( "PopupExit", GUI.root ) );
    exit_popup->init();
    
    std::shared_ptr<PanelPause> pause_panel( new PanelPause( "PanelPause", GUI.root ) );
    pause_panel->init();

    std::shared_ptr<Popup_CA_SHP> ca_shp( new Popup_CA_SHP( "Popup_CA_SHP", GUI.root ) );
    ca_shp->init();
};

void GUI_States::Simulation::exit(){

    GUI.root->get_child( "loading_log" )->set_owner( shared_c_widget(nullptr) );
    GUI.root->get_child( "PanelPause" )->set_owner( shared_c_widget(nullptr) );
    GUI.root->get_child( "ca_shp" )->set_owner( shared_c_widget(nullptr) );
    GUI.root->get_child( "PopupExit" )->set_owner( shared_c_widget(nullptr) );
};

FSM_INITIAL_STATE( GUI_FSM, GUI_States::Start )
