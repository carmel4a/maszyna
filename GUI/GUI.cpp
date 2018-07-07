/*
    This Source Code Form is subject to the
    terms of the Mozilla Public License, v.
    2.0. If a copy of the MPL was not
    distributed with this file, You can
    obtain one at
    http://mozilla.org/MPL/2.0/.
*/

#include <memory>
#include <vector>
#include <string>
#include <typeinfo>

#include "GUI.h"
#include "Logs.h"
#include "CustomWidget.h"

#define FONT_SCALE 2

using namespace nanogui;
class InputScreen;

GUI_ GUI;

//////////
// GUI_ //
//////////

GUI_::GUI_(){

    screen_ = std::make_shared< InputScreen >();
    // Create a gui helper. Creating windows, widgets, etc.
    helper = std::make_shared< FormHelper >( screen() );
    root = nullptr;
    WriteLog( "GUI created." );
};

GUI_::~GUI_(){

    nanogui::shutdown();
    WriteLog("GUI deleted.");
};

Screen* GUI_::screen(){

    return screen_.get();
};

// call on each frame
void GUI_::render(){

    if( may_render && root ){
        update_vars();
        screen_->drawContents();
        screen_->drawWidgets();
    }
};

void GUI_::add_widget(                    std::string name,
                       const shared_customwidget_ptr& s_ptr_custom_widget,
                                          widget_map& where                ){
    s_ptr_custom_widget->init();
    where[name] = s_ptr_custom_widget;
    s_ptr_custom_widget->make();
    update_layout( s_ptr_custom_widget->widget() );
};

void GUI_::update_layout( Widget* of ){
    ( of->parent() )
    ? of->parent()->performLayout( screen_->nvgContext() )
    : root->widget()->performLayout( screen_->nvgContext() );
    screen_->performLayout();
};

void GUI_::remove_widget( std::string name,
                          widget_map& from  ){
        Widget* parent = from[name].get()->widget()->parent();
        parent->removeChild( from[name].get()->widget() );
        update_layout( parent );
        from.erase( name );
};

void GUI_::update_vars(){
    if(root){
        for( auto const& x : root->widgets ){
            if( x.second->may_update ){
                x.second->update();
            }
        }
    }
};

void GUI_::set_root( std::shared_ptr<RootUI> new_root ){

    if( root ){
        screen_->removeChild( root->widget() );
    }
    new_root->init();
    root = new_root;
    root->make();
    update_layout( root->widget() );
    update_layout( screen_.get() );
    screen_->performLayout();
};

template< class T >
T* GUI_::get( std::string name ){
    return dynamic_cast< T* >( widgets[name].get() );
};

/////////////////
// InputScreen //
/////////////////

InputScreen::InputScreen(){

    // Nanogui stuff. Necessary to get resize event.
    setResizeCallback(
        [this]( Vector2i v )->void{
            if((v.x() > 0) && (v.y() > 0))
                resize( v );
        }
    );
    /* not working alternative. Propably better than
       lambda (doesn't create unnamed class).
       setResizeCallback( std::bind( &InputScreen::resize(), this, std::placeholders::_1 ) ); */
};

InputScreen::~InputScreen(){};

void InputScreen::resize( nanogui::Vector2i v ){
    
    if(GUI.root){
        GUI.root->resize( v );
        for( auto const& x : GUI.root->widgets ){
            x.second->resize( v );
        }
    }
};

bool InputScreen::keyboardEvent( int key, int scancode, int action, int modifiers ){

    if( GUI.root->keyboardEvent( key, scancode, action, modifiers )  ) return true;
    if( propagate_key( key, scancode, action, modifiers )  ) return true;
    return false;
};

bool InputScreen::propagate_key( int key, int scancode, int action, int modifiers ){

    if (mFocusPath.size() > 0) {
        for (auto it = mFocusPath.rbegin() + 1; it != mFocusPath.rend(); ++it)
            if ((*it)->focused() && (*it)->keyboardEvent(key, scancode, action, modifiers))
                return true;
    }
    return false;        
};

//////////////////
// DefaultTheme //
//////////////////

DefaultTheme::DefaultTheme( NVGcontext *ctx ):
    Theme( ctx ){
    mStandardFontSize = 16 * FONT_SCALE;
    mButtonFontSize   = 20 * FONT_SCALE;
    mTextBoxFontSize  = 20 * FONT_SCALE;
};

DefaultTheme::~DefaultTheme(){};
