/*
    This Source Code Form is subject to the
    terms of the Mozilla Public License, v.
    2.0. If a copy of the MPL was not
    distributed with this file, You can
    obtain one at
    http://mozilla.org/MPL/2.0/.
*/

/** \file */

#pragma once

#ifndef _CUSTOM_WIDGET_
#define _CUSTOM_WIDGET_

#include <string> // std::string
#include <memory> // std::shared_pt
#include <unordered_map> // std::unordered_map

#include "nanogui/nanogui.h"

class CustomWidget;

/// Shared CustomWidget
typedef std::shared_ptr< CustomWidget > shared_c_widget;
/// std::string : CustomWidget map
typedef std::unordered_map< std::string, shared_c_widget > widget_map;

/// A Base class to create own widgets.
/** It is meant as container for [nanogui::ref to widget](\ref widget_). It
 *  doesn't implement own logic, as creating widget, so in constructor, and
 *  virtual methods user must manually assign nanogui::Widget to nanogui::ref.
 *  ___
 *  ### To create own widget:
 *  1. Create class derivering by CustomWidget.
 *  2. Override make() method.
 *  3. Create object by calling:\n
 *  ```{.cpp}
 *  std::shared_ptr<Derived> object( new Derived( "name", GUI.root ) );
 *  object.init();
 *  ```
 *  **Notes:** 
 *  - Object must be initialized in std::shared_ptr .
 *  - Widget after init WON'T autoupdate. To do that, set \ref may_update to
 *  `true`.
 * 
 *  [Widget data](\ref CustomWidget::Data)
 *  
 *  \todo There should be copy const CustomWidget, but as there are no copy
 *  constr for nanogui::Widget. So it is :effort:.
 *  ```{.cpp}
 *  CustomWidget( const CustomWidget& );
 *  CustomWidget& operator= ( const CustomWidget& );
 *  CustomWidget( CustomWidget&& );
 *  CustomWidget& operator= ( CustomWidget&& );
 *  virtual CustomWidget* create() const = 0;
 *  virtual CustomWidget* clone() const = 0;
 *  ```
 * 
 * ### Ownership and Parentship.
 * Parent is responsible for deleting it's childrens. Most common cause is
 * root (RootUI) which "owns" normal widgets. After deletion Root it
 * deletes its childrens.
 * 
 * \todo Make normal, virtual methods as:
 * ```{.cpp}
 * virtual void load( CustomWidget::Data const * t );
 * virtual CustomWidget::Data const * save();
 * ```
 */
class CustomWidget
    // that may causes some problems..
    : public std::enable_shared_from_this< CustomWidget > {
  public:
    CustomWidget(
            std::string Name, ///< System [name](\ref name) of widget. Must be unique in
                              ///< siblings.
            shared_c_widget Owner = std::shared_ptr<CustomWidget>(nullptr)
                              ///< [Owner](\ref owner) is CustomWidget or null, it has a
                              ///< strong ref to this.
    );
                             
    /// \todo There may be "problem" when something 'll change name in runtime... 
    virtual ~CustomWidget();

    /// Initialize.
    /** %Call that after creating instance of object derivering CustomWidget. */
    void init();
    
    // Childrens //

    /// Add child to this node.
    /** \todo Add contition if widget already not exist. */
    inline void add_child( std::shared_ptr<CustomWidget> sp_cw ) { widgets[sp_cw->name] = sp_cw; };

    /// Get pointer to child. If no child with `name` was fund, returns `nullptr`.
    CustomWidget * get_child( std::string widget_name );

    /// Erase reference to child. It DOESN't delete child, only decrease ref count.
    inline void erase_child( std::string name ) { widgets.erase( name ); };

    // Owner //

    /// Sets \ref owner.
    /** It deletes old reference to old owner, and reference to this from owner.
     *  Then it sets new one. To not set new owner,
     *  pass `std::shared_ptr<CustomWidget>(nullptr)`.
     */
    void set_owner( shared_c_widget sp_cw );

    /// Gets \ref owner.
    /** Propably useless. At leasts now. */
    inline std::shared_ptr<CustomWidget> & get_owner(){ return this->owner; };

    // Widget //

    /// Return raw pointer to main [widget](\ref widget_).
    inline nanogui::Widget* widget(){ return widget_.get(); };

    /// Return nanogui reference to [widget](\ref widget_).
    inline nanogui::ref< nanogui::Widget > widget_ref(){ return widget_; };

    /// Get system \ref name of CustomWidget.
    inline std::string get_name() { return name; };

    /////////////////////
    // Virtual methods //
    /////////////////////

    /// Method to override resize behaviour.
    /** Resize \ref widget_, due to own behaviour. Except make(), don't call
     *  this directly. Default it does nothing. In the future there will be Yoga
     *  node calc methods. Set position, and size here.
     */
    virtual void resize( nanogui::Vector2i v ){};

    /// Change content shown by widget.
    /** Update content of widget, due to onw behaviour.
     *  Currently it's done every frame for every widget,
     *  so don't call that directly. Default - no behaviour.
     */
    virtual void update(){}

    /// Show /ref widget_ .
    /** It also set ref/ may_update for true. */
    virtual void show();

    /// Hide \ref widget_ .
    /** It also set \ref may_update for false and unfocus widget. */
    virtual void hide();

    /// %Widget data.
    /** ### %Widget data
     *  Here \ref widget_ should store raw data necessary to recreate. Size, 
     *  position, content. Default it's empty. See`save()` and `load()` in
     *  derived classes, **'cause currently I can't make it polymorfic.**
     */
    struct Data{
        virtual ~Data() = default;
    } data;

    bool may_update; ///< Will it update next frame?

  protected:

    /// Create \ref widget_.
    /** Overrided method should create (and beeing able to recreate) main
     *  widget, setting values, setting hierarchy of widgets, etc.
     *  It's called in init().
     */
    virtual void make() = 0;

    /// Place where pointers to children are stored.
    /** For them, that CustomWidget is \ref owner.*/
    widget_map widgets;

    /// Pointer to owner.
    /** Owner is responsible for deleting this object. */
    std::shared_ptr<CustomWidget> owner;

    /// System name of object. Must be unique in siblings.
    std::string name;
    nanogui::ref<nanogui::Widget> widget_; ///< Main managed widget.
};

#endif // !_CUSTOM_WIDGET_
