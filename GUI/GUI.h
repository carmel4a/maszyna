/*
    This Source Code Form is subject to the
    terms of the Mozilla Public License, v.
    2.0. If a copy of the MPL was not
    distributed with this file, You can
    obtain one at
    http://mozilla.org/MPL/2.0/.
*/

#pragma once

#ifndef _MAIN_GUI_
#define _MAIN_GUI_

#include <memory.h> // std::shared_pt
#include <unordered_map> // unordered_map
#include <string> // std::string

#include "Yoga.h"

#include "nanogui/nanogui.h"
#include "Logs.h" //debug
#include "RootUI.h"

using namespace nanogui;

class CustomWidget; class LabelArray; class LabelArray;
struct GLFWwindow; struct PrintLine; struct nvgContext;

typedef std::shared_ptr< CustomWidget > shared_customwidget_ptr;
typedef std::unordered_map< std::string, shared_customwidget_ptr > widget_map;
///< Widget map containing names and shared pointers to CustomWidget s.


/// Derivered screen class, to implement own logic.
/** Handles own YGNode, propagate inputs. */
class InputScreen : public nanogui::Screen {
  public:
    InputScreen();
    virtual ~InputScreen();

    bool keyboardEvent( int key, int scancode, int action, int modifiers ) override;

    /** It do NOT resize `Widgets`, it only call `resize()` on CustomWidgets. If
     *  there is need to do sth on every children, it must be implemented.
     */
    void resize( const nanogui::Vector2i v );
  protected:
    /** Default implementation from nanogui::Screen */
    bool propagate_key( int key, int scancode, int action, int modifiers );
};

/// Entry point to user interface.
/** There mustn't be more than one instance of this class. Object of this class
 *  (named `GUI`) is made by 'itself' so you don't want create it by hand.
 */
class GUI_{
  public:
    GUI_();
    ~GUI_();

    /// Post constructor.
    /** %Call after succesfull init of GLFW window. */
    inline void set_window( GLFWwindow* window ){
        screen_->initialize( window, true );
    };
    /// Draw gui.
    /** %Call it every frame. It updates content of widgets. */
    void render();
    
    /// Return nanogui::Screen. Synonym to nanogui::Widget::screen_. 
    /** nanogui::Screen class, handles GLFW window. Represent an glfw window.
     *  It should be an singleton, untill MaSzyna will have one main window.
     *  It may change eg. when mp module will be implemented.
     */
    Screen* screen();
    
    /// Register widget in GUI_.
    /** It only adds CustomWidget to widget array, and calls
     *  make/init methods. Before use you must create object derivering
     *  CustomWidget in `std::shared_ptr` .
     */
    void add_widget(
             std::string name,        ///< System name in widget map.
            const shared_customwidget_ptr& sptr_custom, ///< std::shared_ptr with previously
                                      ///< created CustomWidget.
             widget_map& where        ///< For now it should be GUI_::widgets .
    );
    
    /// Unregister widget in GUI_, and delete it.
    /** It deletes widget from it's parent, then deletes CustomWidget. */
    void remove_widget(
            std::string name, ///< System name of CustomWidget.
            widget_map& from  ///< Widget map where widget is registered. Usually Widgets.
    );
        
    void add_to_layout( const YGNodeRef what, YGNodeRef where );
    void remove_from_layout( const YGNodeRef what, YGNodeRef from );
    
    /** Calls `update()` on root's childrens. */
    void update_vars();

    /** Calls `update()` on root's childrens. */
    void update_layout( Widget* of );

    /// Shortcut to call no virtual methods on objects derivered from CustomWidget .
    /** Usage: `GUI.get<DerivedClass>("name")->method();`\n
     *  If you are creating own widget consider add new virtual method to CustomWidget .
     */
    template< class T >
    T* get( std::string name );
    inline void set_ready( bool b ){ may_render = b; };
    
    bool is_ready(){ return may_render; };

    void set_root( std::shared_ptr<RootUI> new_root );
    
    /// Temporary memory for widgets.
    /** May be deletet. Now we keep CustomWidget s in `GUI.root->widgets`, but
     *  it may be usefull eg. in change root, when we want keep reference to
     *  widget.
     */
    [[depracted]] widget_map widgets;

    /// Root widget.
    /** Root widgets are "normal" widgets, are childrens of GUI_::screen_. They
     *  purpose is implement own "global" input behaviour (key shortcuts,
     *  etc.). There may be understood as different screens/states (Editor,
     *  Menu, Simulation, etc).
     */
    std::shared_ptr< RootUI > root;

  private:
    std::shared_ptr< Screen > screen_;
    std::shared_ptr< FormHelper > helper;
    bool may_render = false;
};

extern GUI_ GUI;

class DefaultTheme : public Theme{
  public:
    DefaultTheme( NVGcontext *ctx );
    ~DefaultTheme();
};

#endif //!_MAIN_GUI_
