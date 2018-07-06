/*
    This Source Code Form is subject to the
    terms of the Mozilla Public License, v.
    2.0. If a copy of the MPL was not
    distributed with this file, You can
    obtain one at
    http://mozilla.org/MPL/2.0/.
*/
//#define Option1;
#define Option2
#define Popup_CA_SHP_FONT_SIZE 50
#define Popup_CA_SHP_FONT_COLOR 255, 0, 0
#define Popup_CA_SHP_TOP_M 10
#define Popup_CA_SHP_RIGHT_M 10

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
#include "OldLayoutSystem.h"

using namespace nanogui;

class GUI_; class CustomWidget;

Popup_CA_SHP::Popup_CA_SHP(){
    may_update = true;
};

Popup_CA_SHP::~Popup_CA_SHP(){};

void Popup_CA_SHP::init(){

    widget_ = new Window( GUI.root->widget(), "" );
};

void Popup_CA_SHP::make(){

    ca = new Label( widget_.get(), "CA" );
    shp = new Label( widget_.get(), "SHP" );
    
    ref< BoxLayout > top_layout = new BoxLayout( Orientation::Horizontal );
    widget_->setLayout( top_layout.get() );

    nanogui::ref< Theme > default_theme = new DefaultTheme( GUI.screen()->nvgContext() );
    widget_->setTheme( default_theme.get() );
    widget_->theme()->mWindowHeaderHeight = 0;
    auto _temp_v = std::vector<Widget*>{
        ca.get(),
        shp.get()
    };
    for(auto const& _v: _temp_v) {
        _v->setLayout( top_layout.get() );
        dynamic_cast< Label* >(_v)->setFont("sans-bold");
        dynamic_cast< Label* >(_v)->setColor( Color( Vector3i( Popup_CA_SHP_FONT_COLOR ) ) );
        dynamic_cast< Label* >(_v)->setFontSize(Popup_CA_SHP_FONT_SIZE);
    }
    GUI.update_layout( widget_.get() );

    resize( Vector2i( Global.iWindowWidth, Global.iWindowHeight ) );
    #ifdef Option1
        hide();
    #endif /* !Option1 */
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
    auto _fix_size =
            shp->preferredSize( GUI.screen()->nvgContext() )
           +ca->preferredSize( GUI.screen()->nvgContext() );
    widget_->setFixedSize( Vector2i( _fix_size.x(), widget_->size().y() ) );
    auto anchor = OldLayout::Anchor( OldLayout::Alignment::End );
    anchor.is_margin_rel = false;
    anchor.margin = Popup_CA_SHP_TOP_M;
    OldLayout::set_x_anchor(
        widget_.get(),
        GUI.screen(),
        anchor
    );
    anchor.mode = OldLayout::Alignment::End;
    anchor.margin = -Popup_CA_SHP_RIGHT_M - widget_->preferredSize( GUI.screen()->nvgContext() ).y();
    OldLayout::set_y_anchor(
        widget_.get(),
        GUI.root->widgets["pause_panel"]->widget(),
        anchor
    );
    GUI.update_layout( widget_.get() );
    anchor.margin = 0;
    OldLayout::set_x_anchor(
        ca.get(),
        widget_.get(),
        anchor
    );
    GUI.update_layout( widget_.get() );
};

void Popup_CA_SHP::update(){
    auto const *controlled { ( Global.pWorld ? Global.pWorld->controlled() : nullptr ) };
    bool _hide = true;
    if( ( controlled != nullptr ) 
     && ( controlled->Mechanik != nullptr ) ) {
        auto const &mover = controlled->MoverParameters;
        if( TestFlag(mover->SecuritySystem.Status, s_aware) ){
            show_CA();
            _hide = false;
        } else hide_CA();
        if( TestFlag(mover->SecuritySystem.Status, s_active) ){
            show_SHP();
            _hide = false;
        } else hide_SHP();
        #ifdef Option1
            ( _hide ) ? hide() : show();
        #endif /* !Option1 */
    }
};

void Popup_CA_SHP::show_CA(){
    ca->setVisible( true );
};

void Popup_CA_SHP::hide_CA(){
    ca->setVisible( false );
};

void Popup_CA_SHP::show_SHP(){
    shp->setVisible( true );
};

void Popup_CA_SHP::hide_SHP(){
    shp->setVisible( false );
};
