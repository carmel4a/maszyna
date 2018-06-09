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
#include <memory.h>
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
        Widget* widget(){ return widget_.get(); };
        nanogui::ref<Widget> widget_ref(){ return widget_; };
        // Called before adding to `GUI::widget` map.
        virtual void init() = 0;
        virtual void make() = 0;

        virtual void show(){ may_update = true; }
        virtual void hide(){ may_update = false; }
        // Except init, you don't want to call this directly.
        virtual void resize( Vector2i v ){};
        virtual void update(){};

        const widget_map* owner;
        bool may_update = false;
        // To print name of widget.
        operator std::string(){
            for( auto const& x : *owner ) {
                if( x.second.get() == this ) return( x.first );
            }
            std::string s = "No *CustomWidget fund in owner!";
            throw s;
        };
        
    protected:
        GUI_::Anchor x_anchor;
        GUI_::Anchor y_anchor;
        nanogui::ref<Widget> widget_;
};

#endif // !_CUSTOM_WIDGET_
