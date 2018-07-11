/*
    This Source Code Form is subject to the
    terms of the Mozilla Public License, v.
    2.0. If a copy of the MPL was not
    distributed with this file, You can
    obtain one at
    http://mozilla.org/MPL/2.0/.
*/

#include <string> // std::string
#include <vector> // std::vector
#include "LabelArray.h"
#include "Globals.h"
#include "Logs.h"
#include "GUI.h"
#include "UI_LoadingScreen.h" // DEbUGc
#include "OldLayoutSystem.h"

using namespace nanogui;

class CustomWidget;

LabelArray::LabelArray(
        std::string Name,
        shared_c_widget Owner,
        bool Transparent,
        std::string Header_Name,
        int Size,
        int FixedW,
        int FixedH,
        std::string Def_text
    ): CustomWidget( Name, Owner )
     , transparent{ Transparent }
     , header_name { Header_Name }
     , init_size{ Size }
     , fixed_w{FixedW}
     , fixed_h{FixedH}
     , def_text{ Def_text }
{
    mode.set( Mode::LIMITED );
};

LabelArray::~LabelArray(){};

void LabelArray::make(){

    ( transparent )
    ? widget_ = new Widget( GUI.root->widget() )
    : widget_ = new Window( GUI.root->widget(), name );
    scroll_panel = new VScrollPanel( widget_ );
    group = new Widget( scroll_panel );
    scroll_panel->setScroll( 1.0F );

    may_update = true;


    ref< BoxLayout > widget_layout = new BoxLayout( Orientation::Vertical, nanogui::Alignment::Fill, 25 );
    widget_->setLayout( widget_layout.get() );

    ref< BoxLayout > scroll_layout = new BoxLayout( Orientation::Horizontal, nanogui::Alignment::Minimum, 0 );
    scroll_panel->setLayout( scroll_layout.get() );

    ref< BoxLayout > group_layout = new BoxLayout( Orientation::Vertical, nanogui::Alignment::Minimum, 0, 10  );
    group->setLayout( group_layout.get() );

    nanogui::ref< Theme > default_theme = new DefaultTheme( GUI.screen()->nvgContext() );
    widget_->setTheme( default_theme );

    GUI.update_layout( widget_.get() );
    resize( Vector2i( Global.iWindowWidth, Global.iWindowHeight ) );
};

void LabelArray::show(){

    widget_->setVisible( true );
    GUI.update_layout( widget_.get() );
    may_update = true;
};

void LabelArray::hide(){
    
    widget_->setVisible( false );
    GUI.update_layout( widget_.get() );
    may_update = false;
};

void LabelArray::resize( Vector2i v ){
    
    int max_x, max_y;
    (fixed_w == -1) ? max_x = v.x()/2 : max_x = fixed_w;
    (fixed_h == -1) ? max_y = v.y()/2 : max_y = fixed_h;

    // Set widget_
    widget_->setFixedWidth(max_x);
    widget_->setFixedHeight(max_y);
    auto anchor = OldLayout::Anchor( OldLayout::Alignment::Centered );
    OldLayout::set_x_anchor(
            widget_.get(), GUI.screen(), anchor );
    OldLayout::set_y_anchor(
            widget_.get(), GUI.screen(), anchor );

    GUI.update_layout( widget_.get() );
    scroll_panel->setFixedHeight( widget_->size().y() - widget_->theme()->mWindowHeaderHeight -10 -25 );
    GUI.update_layout( scroll_panel.get() );
    GUI.update_layout( group.get() );
};

void LabelArray::update(){

    if( !transparent ){
        dynamic_cast< Window* >(widget_.get())->setTitle( name );
    }
    // Here we can add list with references to strings..
    // to auto update content.
};

void LabelArray::push_line( std::string text ){
    //float old_scroll = scroll_panel->scroll();
    label_array.push_back( ref<Label>() );
    label_array.back() = new Label( group.get(), "" );
    label_array.back()->setCaption( text );
    label_array.back()->setFixedWidth( scroll_panel->size().x() - 10 - 10 );    
    
    if( init_size < group->children().size() ){
        /*
        group->removeChild( 
            group->childIndex( 
                label_array.front().get()
             )
            );
        */
        if( mode.test( Mode::LIMITED ) ){
            group->removeChild( 0 );
            label_array.front() = nullptr;
            label_array.pop_front();
        }
    }
    GUI.update_layout( group.get() );
    scroll_panel->setScroll( 1.0F );
};

void LabelArray::load( LabelArray::Data const * t ){

    clear();
    for( auto const & x : t->labels ){
        push_line( x );
    }
};

LabelArray::Data & LabelArray::save(){

    data.labels.clear();
    for( auto const & x : label_array ){
        data.labels.push_back( x->caption() );
    }
    return data;
};

void LabelArray::clear(){

    for( auto const & x : label_array ){
        widget_->removeChild( x.get() );
    }
};

void LoadingLog::update(){ return; };

void LoadingLog::init(){
    ( transparent )
    ? widget_ = new Widget( GUI.screen() )
    : widget_ = new Window( GUI.screen(), name );
    scroll_panel = new VScrollPanel( widget_ );
    group = new Widget( scroll_panel );
    may_update = true;
};

void LoadingLog::make(){

    //ref< BoxLayout > top_layout = new BoxLayout( Orientation::Vertical );

    //widget_->setLayout( top_layout.get() );
    //group->setLayout( top_layout.get() );

    nanogui::ref< Theme > default_theme = new DefaultTheme( GUI.screen()->nvgContext() );
    widget_->setTheme( default_theme );
    //panel->theme()->mWindowHeaderHeight = 0;

    GUI.update_layout( widget_.get() );
    resize( Vector2i( Global.iWindowWidth, Global.iWindowHeight ) );
};
