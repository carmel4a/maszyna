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
#include "Yoga.h"

using namespace nanogui;

class GUI_; class CustomWidget; class PopupExitPanel;

PanelPause::PanelPause(){

};

PanelPause::~PanelPause(){
};

void PanelPause::init(){

    widget_ = new Widget( GUI.root->widget() );
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

    
    YGNodeStyleSetPosition( YG_node, YGEdgeTop, 10 );
    YGNodeStyleSetPosition( YG_node, YGEdgeRight, 10 );

    YGNodeStyleSetPositionType( YG_node, YGPositionTypeAbsolute );
    
    YGNodeStyleSetMinWidth( YG_node, widget_->preferredSize( GUI.screen()->nvgContext() ).x() );
    YGNodeStyleSetMinHeight( YG_node, widget_->preferredSize( GUI.screen()->nvgContext() ).y() );
    YGNodeStyleSetWidth( YG_node, widget_->preferredSize( GUI.screen()->nvgContext() ).x() );
    YGNodeStyleSetHeight( YG_node, widget_->preferredSize( GUI.screen()->nvgContext() ).y() );

    resize( Vector2i( Global.iWindowWidth, Global.iWindowHeight ) );
};

void PanelPause::update(){

    if( Global.iPause > 0 ) show();
    else hide();
};

void PanelPause::hide(){

    widget_->setVisible( false );
    GUI.update_layout( widget_.get() );
    widget_->setFocused( false );
};
