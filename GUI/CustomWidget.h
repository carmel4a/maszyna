/*
    This Source Code Form is subject to the
    terms of the Mozilla Public License, v.
    2.0. If a copy of the MPL was not
    distributed with this file, You can
    obtain one at
    http://mozilla.org/MPL/2.0/.
*/
#pragma once

#ifndef _CUSTOM_WIDGET_
#define _CUSTOM_WIDGET_

#include <string>

#include "GUI.h"
#include "nanogui/nanogui.h"

using namespace nanogui;
class GUI_;
class CustomWidget;

typedef std::shared_ptr< CustomWidget > shared_custom_widget;
typedef std::unordered_map< std::string, shared_custom_widget > widget_map;

/*! A Base class to create own widgets. It is meant as container for nanogui::ref to 
 * widget. It doesn't implement own logic, as creating widget, so in overrided
 * constructor, and virtual methods user must include manually assign nanogui::Widget
 * to nanogui::ref.
 */
class CustomWidget{
    
    public:
        CustomWidget(){};
        virtual ~CustomWidget(){};
        Widget* get_widget(){ return widget.get(); };
        nanogui::ref<Widget> get_widget_ref(){ return widget; };
        virtual void init() = 0;
        virtual void init_layout() = 0;
        virtual void make() = 0;
        //virtual void resize() = 0;
        
        // Top screen widgets must override this method
        virtual AdvancedGridLayout::Anchor default_anchor(){ return( nanogui::AdvancedGridLayout::Anchor( 0, 0 ) ); };
        virtual void show(){};
        virtual void hide(){};
        widget_map widgets; //pls don't use it on top-level widgets.
        
    protected:
        GUI_::Anchor x_anchor;
        GUI_::Anchor y_anchor;
        nanogui::ref<Widget> widget;
};

#endif // !_CUSTOM_WIDGET_
