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
#define DEBUG

using namespace nanogui;

class CustomWidget;

LabelArray::LabelArray(
        bool Transparent,
        std::string Name,
        int Size,
        int FixedW,
        int FixedH,
        std::string Def_text
    ):
    transparent{ Transparent },
    name { Name },
    init_size{ Size },
    fixed_w{FixedW},
    fixed_h{FixedH},
    def_text{ Def_text }
{};

LabelArray::~LabelArray(){};

void random_str(char *s, const int len){
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    for (int i = 0; i < len; ++i) {
        s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    };
    s[len] = 0;
}

void LabelArray::init(){
    ( transparent )
    ? widget_ = new Widget( GUI.get_screen() )
    : widget_ = new Window( GUI.get_screen(), name );
    scroll_panel = new VScrollPanel( widget_ );
    group = new Widget( scroll_panel );
    ref< Label > _temp;
    #ifdef DEBUG
        def_text = "test";
        init_size = 20;
    #endif /* !DEBUG */
    for(int i = 0; i < init_size; i++){
        char _temp_s[70];
        random_str(_temp_s, 70);
        const std::string _final_text = std::string( def_text + " " + (std::string)_temp_s );
        push_line( _final_text );
        #ifdef DEBUG
            WriteLog( _final_text );
        #endif /* !DEBUG */
    }
    may_update = true;
};

void LabelArray::make(){

    ref< BoxLayout > widget_layout = new BoxLayout( Orientation::Vertical, nanogui::Alignment::Fill, 25 );
    widget_->setLayout( widget_layout.get() );

    ref< BoxLayout > scroll_layout = new BoxLayout( Orientation::Horizontal, nanogui::Alignment::Minimum, 0 );
    scroll_panel->setLayout( scroll_layout.get() );

    ref< BoxLayout > group_layout = new BoxLayout( Orientation::Vertical, nanogui::Alignment::Minimum, 0, 10  );
    group->setLayout( group_layout.get() );

    nanogui::ref< Theme > default_theme = new DefaultTheme( GUI.get_screen()->nvgContext() );
    widget_->setTheme( default_theme );


    GUI.update_layout( widget_.get() );
    resize( Vector2i( Global.iWindowWidth, Global.iWindowHeight ) );
};

void LabelArray::show(){

    widget_->setVisible( true );
    GUI.update_layout( widget_.get() );
};

void LabelArray::hide(){
    
    widget_->setVisible( false );
    GUI.update_layout( widget_.get() );
};

void LabelArray::resize( Vector2i v ){
    int max_x, max_y;
    if(fixed_w == -1) max_x = v.x()/2;
    else max_x = fixed_w;
    if(fixed_h == -1) max_y = v.y()/2;
    else max_y = fixed_h;

    widget_->setFixedWidth(max_x);
    widget_->setFixedHeight(max_y);
    // Set widget_
    auto anchor = GUI_::Anchor( GUI.Alignment::Centered );
    GUI.set_x_anchor(
            widget_.get(), GUI.get_screen(), anchor );
    GUI.set_y_anchor(
            widget_.get(), GUI.get_screen(), anchor );

    GUI.update_layout( widget_.get() );
    scroll_panel->setFixedHeight( widget_->size().y() - widget_->theme()->mWindowHeaderHeight -10 -25 );
    GUI.update_layout( scroll_panel.get() );
    GUI.update_layout( group.get() );
    
};

void LabelArray::update(){

    if( !transparent ){
        dynamic_cast< Window* >(widget_.get())->setTitle( name );
    }
    char _temp_s[100];
    random_str(_temp_s, 100);
    const std::string _final_text = std::string( def_text + " " + (std::string)_temp_s );
    push_line( _final_text );

};

void LabelArray::push_line( std::string text ){

    ref<Label> _temp = new Label( group.get(), "" );
    
    _temp->setCaption( text );
    label_array.emplace_back( _temp );
    _temp->setFixedWidth( scroll_panel->size().x() - 10 - 10 );    
    if( init_size < label_array.size() ){
        label_array.front()->parent()->removeChild( label_array.front().get() );
        label_array.pop_front();
    }
    GUI.update_layout( group.get() );
};



void LoadingLog::update(){ return; };


void LoadingLog::init(){
    ( transparent )
    ? widget_ = new Widget( GUI.get_screen() )
    : widget_ = new Window( GUI.get_screen(), name );
    scroll_panel = new VScrollPanel( widget_ );
    group = new Widget( scroll_panel );
    ref< Label > _temp;
    def_text = "test";
    for(int i = 0; i < init_size; i++){
        char _temp_s[25];
        random_str(_temp_s, 25);
        const std::string _final_text = std::string( def_text + " " + (std::string)_temp_s );
        push_line( _final_text );
        WriteLog( _final_text );
    }
    may_update = true;
};

void LoadingLog::make(){

    //ref< BoxLayout > top_layout = new BoxLayout( Orientation::Vertical );

    //widget_->setLayout( top_layout.get() );
    //group->setLayout( top_layout.get() );

    nanogui::ref< Theme > default_theme = new DefaultTheme( GUI.get_screen()->nvgContext() );
    widget_->setTheme( default_theme );
    //panel->theme()->mWindowHeaderHeight = 0;

    GUI.update_layout( widget_.get() );
    resize( Vector2i( Global.iWindowWidth, Global.iWindowHeight ) );
};
