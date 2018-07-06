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
#include "OldLayoutSystem.h"

using namespace nanogui;

class GUI_; class CustomWidget; class PopupExitPanel;

PanelPause::PanelPause(){

};

PanelPause::~PanelPause(){
};

void PanelPause::init(){

    widget_ = new Window( GUI.root->widget(), "" );
    may_update = true;
};

void PanelPause::make(){

    ref< BoxLayout > top_layout = new BoxLayout( Orientation::Horizontal );
    top_layout->setMargin( 5 ); 
    widget_->setLayout( top_layout.get() );
    
    widget_->theme()->mWindowHeaderHeight = 0;

    nanogui::ref< Label > label = new Label( widget_.get(), "PAUSED" );
    GUI.update_layout( widget_.get() );

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

void PanelPause::resize( Vector2i v ){

    auto anchor = OldLayout::Anchor( OldLayout::Alignment::End );
    anchor.is_margin_rel = false;
    anchor.margin = 10;
    OldLayout::set_x_anchor(
        widget_.get(),
        GUI.screen(),
        anchor
    );
    anchor.mode = OldLayout::Alignment::Begin;
    OldLayout::set_y_anchor(
        widget_.get(),
        GUI.screen(),
        anchor
    );
}