/*
    This Source Code Form is subject to the
    terms of the Mozilla Public License, v.
    2.0. If a copy of the MPL was not
    distributed with this file, You can
    obtain one at
    http://mozilla.org/MPL/2.0/.
*/

#include <vector>
#include <functional>
#include <memory>

#include "GUI.h"
#include "CustomWidget.h"
#include "PanelPause.h"
#include "Globals.h"

using namespace nanogui;

class GUI_; class CustomWidget; class PopupExitPanel;

PanelPause::PanelPause(){

};

PanelPause::~PanelPause(){};

void PanelPause::init(){

    widget = new Widget( GUI.get_screen() );
    panel = new Window( widget, "" );
    /*
    GUI.register_widget( *widget );
    */
};

void PanelPause::init_layout(){
    
    ref< BoxLayout > top_layout = new BoxLayout( Orientation::Horizontal );

    widget->setLayout( top_layout.get() );
    
    nanogui::ref< BoxLayout > layout = new BoxLayout( nanogui::Orientation::Vertical );
    panel->setLayout( layout.get() );
    nanogui::ref< Theme > default_theme = new DefaultTheme( GUI.get_screen()->nvgContext() );
    panel->setTheme( default_theme );
    panel->theme()->mWindowHeaderHeight = 0;
};

void PanelPause::make(){

    nanogui::ref< Label > label = new Label( panel.get(), "PAUSED" );
  //  dynamic_cast< AdvancedGridLayout* >( GUI.get_screen()->layout() )->setAnchor( widget.get(), default_anchor() );
    GUI.update_layout( panel.get() );
    GUI.update_layout( widget.get() );

    resize( Vector2i( Global.iWindowWidth, Global.iWindowHeight ) );
};

void PanelPause::show(){

    widget->setVisible( true );
    GUI.update_layout( widget.get() );
};

void PanelPause::hide(){
    
    widget->setVisible( false );
    GUI.update_layout( widget.get() );
};

void PanelPause::resize( Vector2i v ){
    auto anchor = GUI_::Anchor( GUI.Alignment::End );
    anchor.is_margin_rel = false;
    anchor.margin = 10;
    GUI.set_x_anchor(
        widget.get(),
        GUI.get_screen(),
        anchor
    );
    anchor.mode = GUI.Alignment::Begin;
    GUI.set_y_anchor(
        widget.get(),
        GUI.get_screen(),
        anchor
    );
}