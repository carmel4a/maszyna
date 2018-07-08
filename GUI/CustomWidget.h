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
#include <memory> // std::shared_pt
#include <unordered_map> // std::unordered_map

#include "nanogui/nanogui.h"

using namespace nanogui;

class CustomWidget;

typedef std::shared_ptr< CustomWidget > shared_customwidget_ptr;
typedef std::unordered_map< std::string, shared_customwidget_ptr > widget_map;

/// A Base class to create own widgets.
/** It is meant as container for nanogui::ref to widget. It doesn't implement
 *  own logic, as creating widget, so in constructor, and virtual methods user
 *  must manually assign nanogui::Widget to nanogui::ref.  
 */
class CustomWidget{
  public:
    CustomWidget();
    virtual ~CustomWidget() = default;

    CustomWidget( const CustomWidget& );
    CustomWidget& operator= ( const CustomWidget& ); // TODO
    CustomWidget( CustomWidget&& );                  // TODO
    CustomWidget& operator= ( CustomWidget&& );      // TODO

    /** To print name of widget. */
    operator std::string();

    /// Method to override resize behaviour.
    /** Resize widget, due to onw behaviour. Except init, you don't want to
     *  call this directly. Default it calls `calc_layout`. Set position, size,
     *  etc here.
     */
    virtual void resize( Vector2i v );

    /// Calculate and set size/position of widget.

    /** Update content of widget, due to onw behaviour.
     *  Currently it's done every frame for every widget,
     *  so don't call that directly.
     */
    virtual void update(){};

    /////////////
    // Getters //
    /////////////

    /** Return raw pointer to main Widget */
    inline Widget* widget(){ return widget_.get(); };
    /** Return nanogui reference to Widget */
    inline nanogui::ref<Widget> widget_ref(){ return widget_; };

    //////////////////////////
    // Pure virtual methods //
    //////////////////////////

    // virtual CustomWidget* create() const = 0; // TODO
    virtual CustomWidget* clone() const = 0;
    /** Called before adding to `GUI::widget` map. */
    virtual void init() = 0;
    /** Called after adding to `GUI::widget` map. */
    virtual void make() = 0;

    /////////////////////
    // Virtual methods //
    /////////////////////

    virtual void show();
    virtual void hide();

    bool may_update; ///< Will it update next frame?

    [[depracted]]
    const widget_map* owner; ///< Where is shared_ptr?
    
    widget_map widgets;
  protected:
    nanogui::ref<Widget> widget_; ///< Main managed widget.
};

#endif // !_CUSTOM_WIDGET_
