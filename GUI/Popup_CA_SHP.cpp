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
#include "Popup_CA_SHP.h"
#include "Globals.h"

#include "World.h"
#include "DynObj.h"
#include "Driver.h"

using namespace nanogui;

class GUI_; class CustomWidget;

Popup_CA_SHP::Popup_CA_SHP(){
    may_update = true;
};

Popup_CA_SHP::~Popup_CA_SHP(){};

void Popup_CA_SHP::init(){

    widget_ = new Label( GUI.get_screen(), "CA/SHP" );
};

void Popup_CA_SHP::make(){

    ref< BoxLayout > top_layout = new BoxLayout( Orientation::Horizontal );
    widget_->setLayout( top_layout.get() );

    nanogui::ref< Theme > default_theme = new DefaultTheme( GUI.get_screen()->nvgContext() );
    widget_->setTheme( default_theme.get() );
    dynamic_cast< Label* >(widget_.get())->setFont("sans-bold");
    dynamic_cast< Label* >(widget_.get())->setColor( Color( Vector3i( 255, 0, 0 ) ) );
    widget_->theme()->mTextBoxFontSize = 128;
    widget_->theme()->mStandardFontSize = 128;
    widget_->theme()->mButtonFontSize = 128;
    dynamic_cast< Label* >(widget_.get())->setFontSize(128);
    GUI.update_layout( widget_.get() );

    resize( Vector2i( Global.iWindowWidth, Global.iWindowHeight ) );
    hide();
};

void Popup_CA_SHP::show(){

    widget_->setVisible( true );
    GUI.update_layout( widget_.get() );
};

void Popup_CA_SHP::hide(){
    
    widget_->setVisible( false );
    GUI.update_layout( widget_.get() );
};

void Popup_CA_SHP::resize( Vector2i v ){
    auto anchor = GUI_::Anchor( GUI.Alignment::Centered );
    GUI.set_x_anchor(
        widget_.get(),
        GUI.get_screen(),
        anchor
    );
    GUI.set_y_anchor(
        widget_.get(),
        GUI.get_screen(),
        anchor
    );
};

void Popup_CA_SHP::update(){

    auto const *controlled { ( Global.pWorld ? Global.pWorld->controlled() : nullptr ) };
    if( ( controlled != nullptr ) 
     && ( controlled->Mechanik != nullptr ) ) {
        auto const &mover = controlled->MoverParameters;
        if( TestFlag(mover->SecuritySystem.Status, s_aware)
        || TestFlag(mover->SecuritySystem.Status, s_active) ){
            show();
        } else {
            hide();
        }
    }
};
