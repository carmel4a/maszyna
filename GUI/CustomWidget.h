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

#include <string> // std::string
#include <memory.h> // std::shared_pt
#include <unordered_map> // std::unordered_map

#include "GUI.h"
#include "nanogui/nanogui.h"

using namespace nanogui;

class GUI_; class CustomWidget;

typedef std::shared_ptr< CustomWidget > shared_custom_widget;
typedef std::unordered_map< std::string, shared_custom_widget > widget_map;

/// A Base class to create own widgets.
/**
  It is meant as container for nanogui::ref to 
  widget. It doesn't implement own logic, as creating widget, so in overrided
  constructor, and virtual methods user must include manually assign nanogui::Widget
  to nanogui::ref.
*/
class CustomWidget{
    
  public:
    CustomWidget(){};
    virtual ~CustomWidget(){};

    /////////////
    // Getters //
    /////////////

    /** Return raw pointer to main Widget */
    Widget* widget(){ return widget_.get(); };
    /** Return nanogui reference to Widget */
    nanogui::ref<Widget> widget_ref(){ return widget_; };

    //////////////////////////
    // Pure virtual methods //
    //////////////////////////

    /** Called before adding to `GUI::widget` map. */
    virtual void init() = 0;
    /** Called after adding to `GUI::widget` map. */
    virtual void make() = 0;

    /////////////////////
    // Virtual methods //
    /////////////////////

    virtual void show(){ may_update = true; }
    virtual void hide(){ may_update = false; }

    /** Resize widget, due to onw behaviour.
     *  Except init, you don't want to call this directly.
     *  Set anchor (position, size - layout) here.
     */
    virtual void resize( Vector2i v ){};

    /** Update content of widget, due to onw behaviour.
     *  Currently it's done every frame for every widget,
     *  so don't call that directly.
     */
    virtual void update(){};
    bool may_update = false;

    const widget_map* owner;
    /** To print name of widget. */
    operator std::string(){
        for( auto const& x : *owner ) {
            if( x.second.get() == this ) return( x.first );
        }
        std::string s = "No *CustomWidget fund in owner!";
        throw s;
    };
        
  protected:
    nanogui::ref<Widget> widget_; /**< Main managed widget. */
};

#endif // !_CUSTOM_WIDGET_
