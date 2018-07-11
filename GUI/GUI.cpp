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
#include "InputScreen.h"

#define FONT_SCALE 2

using namespace nanogui;

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

// call on each frame
void GUI_::render(){

    if( may_render && root ){
        update_vars();
        screen_->drawContents();
        screen_->drawWidgets();
    }
};

void GUI_::update_layout( Widget* of ){

    ( of->parent() )
    ? of->parent()->performLayout( screen_->nvgContext() )
    : root->widget()->performLayout( screen_->nvgContext() );
    screen_->performLayout();
};

void GUI_::set_root( std::shared_ptr<RootUI> new_root ){

    if( ! new_root ) return;
    if( new_root.get() != root.get() ){
        if( root ){
            root->delete_();
        }
        root = new_root;
        update_layout( screen_.get() );
        screen_->performLayout();
    }
};

template< class T >
T* GUI_::get( std::string name ){
    return dynamic_cast< T* >( widgets[name].get() );
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
