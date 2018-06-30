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

#include "tinyfsm.hpp"
#include "Yoga.h"

#include "nanogui/nanogui.h"
#include "Logs.h" //debug

using namespace nanogui;

class CustomWidget; class LabelArray; class LabelArray;
struct GLFWwindow; struct PrintLine;

typedef std::shared_ptr< CustomWidget > shared_customwidget_ptr;
typedef std::unordered_map< std::string, shared_customwidget_ptr > widget_map;

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
    inline void set_main_window( GLFWwindow* window ){
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

    void update_vars();
    void update_layout( Widget* of );

    widget_map widgets; ///< Widget map containing names and shared pointers to CustomWidget s.
    std::shared_ptr< CustomWidget > root;
    /// Shortcut to call no virtual methods on objects derivered from CustomWidget .
    /** Usage: `GUI.get<DerivedClass>("name")->method();`
     *  If you are creating own widget consider add new virtual method to CustomWidget .
     */
    template< class T >
    T* get( std::string name );
    
    enum Alignment : short{
        Begin,    ///< up to down, left to right
        Centered, ///< centered, prefered size
        End,      ///< down to up, right to left
        Fill      ///< fill this axis
    };

    struct Anchor{
        Anchor( Alignment mode = Alignment::Begin ) :
        mode{mode} {};
        bool is_margin_rel = true;
        int margin = 0;
        float margin_rel = 0;
        Alignment mode;
    };
    
    inline void set_x_anchor( 
            Widget* what,
            Widget* to,
            GUI_::Anchor anchor_x
    ){ _set_axis_anchor( *what, *to, anchor_x, 0 ); };
    
    inline void set_y_anchor( 
            Widget* what,
            Widget* to,
            GUI_::Anchor anchor_y
    ){ _set_axis_anchor( *what, *to, anchor_y, 1 ); };

    /////////
    // FSM //
    /////////

    struct GUI_Init : public tinyfsm::Event {};
    struct SceneLoaded : public tinyfsm::Event {};
    
    struct GUI_FSM : public tinyfsm::Fsm<GUI_FSM>{
            
        //virtual void react(tinyfsm::Event const &) { };
        virtual void react( GUI_Init const & ) {  };
        virtual void react( PrintLine const & ) {  };
        virtual void react( SceneLoaded const & ) {  };

        virtual void entry(void) { };
        virtual void exit(void)  { };
    };

    struct Start : GUI_FSM{
        void react( GUI_Init const & ) override;
        void entry() override;
        void exit() override;
    };

    struct LoadingScreen : GUI_FSM{
        void react( PrintLine const & e ) override;
        void react( SceneLoaded const & ) override;
        void entry() override;
        void exit() override;
    };

    struct Simulation : GUI_FSM{
        void entry() override;
    };
    bool is_ready(){ return may_render; };
  private:
    void _set_axis_anchor( 
            Widget& what,
            Widget& to,
            GUI_::Anchor anchor,
            short axis
    );
        
    template< typename T = Vector2i >
    inline auto _get_axis( T t, int i ){
        return i == 0 ? t.x() : t.y();
    };

    template< typename T = Vector2i >
    inline T _get_rel_to_axis( T v1, T v2 , int axis ){
        return (axis == 0) ?
            T(
               _get_axis( v1, axis ),
               _get_axis( v2, !axis ) ) :
            T(
               _get_axis( v2, !axis ),
               _get_axis( v1, axis ) );
    };
    
    std::shared_ptr< Screen > screen_;
    std::shared_ptr< FormHelper > helper;
    bool may_render = false; // dono if it'll be necesary.
};

extern GUI_ GUI;

class InputScreen : public nanogui::Screen {
  public:
    InputScreen();
    virtual ~InputScreen();
    bool keyboardEvent(int key, int scancode, int action, int modifiers) override;
    void resize( const nanogui::Vector2i v );
    YGNodeRef YG_node;
};

class DefaultTheme : public Theme{
  public:
    DefaultTheme( NVGcontext *ctx );
    ~DefaultTheme();
};

#endif //!_MAIN_GUI_
