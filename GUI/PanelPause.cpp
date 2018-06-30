/*
    This Source Code Form is subject to the
    terms of the Mozilla Public License, v.
    2.0. If a copy of the MPL was not
    distributed with this file, You can
    obtain one at
    http://mozilla.org/MPL/2.0/.
*/


#include "GUI.h"
#include "CustomWidget.h"
#include "PanelPause.h"
#include "Globals.h"

using namespace nanogui;

class GUI_; class CustomWidget; class PopupExitPanel;

PanelPause::PanelPause(){

};

PanelPause::~PanelPause(){
};

void PanelPause::init(){

    widget_ = new Widget( GUI.screen() );
    panel = new Window( widget_, "" );
    may_update = true;
};

void PanelPause::make(){

    ref< BoxLayout > top_layout = new BoxLayout( Orientation::Horizontal );

    widget_->setLayout( top_layout.get() );
    
    nanogui::ref< BoxLayout > layout = new BoxLayout( nanogui::Orientation::Vertical );
    panel->setLayout( layout.get() );
    nanogui::ref< Theme > default_theme = new DefaultTheme( GUI.screen()->nvgContext() );
    panel->setTheme( default_theme );
    panel->theme()->mWindowHeaderHeight = 0;

    
    nanogui::ref< Label > label = new Label( panel.get(), "PAUSED" );
    GUI.update_layout( panel.get() );
    GUI.update_layout( widget_.get() );

    resize( Vector2i( Global.iWindowWidth, Global.iWindowHeight ) );
};

void PanelPause::show(){

    widget_->setVisible( true );
    GUI.update_layout( widget_.get() );
};

void PanelPause::hide(){
    
    widget_->setVisible( false );
    GUI.update_layout( widget_.get() );
};

void PanelPause::resize( Vector2i v ){

    auto anchor = GUI_::Anchor( GUI.Alignment::End );
    anchor.is_margin_rel = false;
    anchor.margin = 10;
    GUI.set_x_anchor(
        widget_.get(),
        GUI.get_screen(),
        anchor
    );
    anchor.mode = GUI.Alignment::Begin;
    GUI.set_y_anchor(
        widget_.get(),
        GUI.get_screen(),
        anchor
    );
}

void PanelPause::update(){

    if( Global.iPause > 0 ) show();
    else hide();
};