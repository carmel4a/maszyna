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

typedef std::shared_ptr< CustomWidget > shared_c_widget;
typedef std::unordered_map< std::string, shared_c_widget > widget_map;

/// A Base class to create own widgets.
/** It is meant as container for nanogui::ref to widget. It doesn't implement
 *  own logic, as creating widget, so in constructor, and virtual methods user
 *  must manually assign nanogui::Widget to nanogui::ref.  
 */
class CustomWidget
    : public std::enable_shared_from_this< CustomWidget > {
  public:
    CustomWidget(
            std::string Name,
            shared_c_widget Owner = nullptr
    );

    virtual ~CustomWidget();

    CustomWidget( const CustomWidget& );
    CustomWidget& operator= ( const CustomWidget& ); // TODO
    CustomWidget( CustomWidget&& );                  // TODO
    CustomWidget& operator= ( CustomWidget&& );      // TODO

    void init();

    inline void add_child( std::shared_ptr<CustomWidget> sp_cw ) { widgets[sp_cw->name] = sp_cw; };
    inline CustomWidget * get_child( std::string widget_name ) { return widgets[widget_name].get(); };
    inline void erase_child( std::string name ) { widgets.erase( name ); };

    void set_owner( shared_c_widget sp_cw );
    inline std::shared_ptr<CustomWidget> & get_owner(){ return this->owner; };

    /** Return raw pointer to main Widget */
    inline Widget* widget(){ return widget_.get(); };
    /** Return nanogui reference to Widget */
    inline nanogui::ref<Widget> widget_ref(){ return widget_; };

    inline std::string get_name() { return name; };
    //////////////////////////
    // Pure virtual methods //
    //////////////////////////

    // virtual CustomWidget* create() const = 0; // TODO
    virtual CustomWidget* clone() const = 0;
    /** Called after adding to `GUI::widget` map. */
    virtual void make() = 0;

    /////////////////////
    // Virtual methods //
    /////////////////////

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

    virtual void show();
    virtual void hide();

    struct Data{
        virtual ~Data() = default;
    } data; 
    
    // YOLO, Nope virtual. Bay!
    // virtual void load( CustomWidget::Data const * t );
    // virtual CustomWidget::Data const * save();

    bool may_update; ///< Will it update next frame?

    
  protected:
    widget_map widgets;
    std::shared_ptr<CustomWidget> owner; ///< Where is shared_ptr?
    std::string name;
    nanogui::ref<Widget> widget_; ///< Main managed widget.
};

#endif // !_CUSTOM_WIDGET_
