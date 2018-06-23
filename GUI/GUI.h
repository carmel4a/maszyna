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

#include <memory.h>
#include <unordered_map>
#include <string>
#include <vector>

#include "tinyfsm.hpp"

using namespace nanogui;

class CustomWidget;

typedef std::shared_ptr< CustomWidget > shared_custom_widget;
typedef std::unordered_map< std::string, shared_custom_widget > widget_map;

class GUI_{

    public:
        GUI_();
        ~GUI_();

        /*! Call after succesfull init of glfw window. */
        void init(GLFWwindow* window);
        
        /*! Call it every frame. */
        void draw_gui();
        [[deprecated]]
        void update_all_vars();
        
        /*! nanogui::Screen class, handles GLFW window. Represent an glfw window.
        It should be an singleton, untill MaSzyna will have one main window.
        It may change eg. when mp module will be implemented. */
        Screen* get_screen();
        FormHelper* get_helper();
        
        void add_widget(                 std::string name,
                         const shared_custom_widget& sptr_custom,
                                         widget_map& where       );
        
        void remove_widget( std::string name,
                            widget_map& from );
        void update_vars();
        void update_layout( Widget* of );
        [[deprecated]]
        void focus_helper_on( Window& window );
        widget_map widgets;
        template< class T >
        T* get( std::string name );

        enum Alignment : short{
            Begin,
            Centered,
            End,
            Fill
        };

        // Vector2i Vector2f Vector2d
        template< typename T = Vector2i >
        struct Rect2{
            T pos;
            T size;

            // false rel Position in px from top left to botton right.
            //           Size - in px from Position in the same dir.
            // true rel  Position in % of parent (scren/widget) from top
            // left to botton right.
            //           Size - in px from parent in the same dir.
            /////////////////////////////////////////////////////////
            //   |
            // --+############ -> x+
            //   #...........#   Pos ( 1/13, 3/7 )
            //   #..####### .#   Size ( 6/13, 3/7 )
            //   #..#,,,,,# .#   
            //   #..####### .#   + - origin
            //   #...........#
            //   #############
            //   v 
            //   y+
            bool rel = false;
        };
        // begin: up to down, left to right
        // end: down to up, right to left
        struct Anchor{
            Anchor( Alignment mode = Alignment::Begin ) :
            mode{mode} {};
            bool is_rect_rel = true;
            Vector2i start = Vector2i( 0, 0);
            Vector2i end = Vector2i( 1, 1 );
            bool is_margin_rel = true;
            int margin = 0;
            float margin_rel = 0;
            Alignment mode;
        };
        
        void set_x_anchor( 
                Widget* what,
                Widget* to,
                GUI_::Anchor anchor_x
        );
        
        void set_y_anchor( 
                Widget* what,
                Widget* to,
                GUI_::Anchor anchor_y

    /////////
    // FSM //
    /////////

    struct GUI_Init : public tinyfsm::Event {};
    struct SceneLoaded : public tinyfsm::Event {};
    
    struct GUI_FSM : public tinyfsm::Fsm<GUI_FSM>{
            
        //virtual void react(tinyfsm::Event const &) { };
        virtual void react( GUI_Init const & ) {  };
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
        void react( SceneLoaded const & ) override;
        void entry() override;
        //void exit() override;
    };

    struct Simulation : GUI_FSM{
        //void react( JakisEvent const & ) override;
        void entry() override;
    };

    private:
        void _set_axis_anchor( 
                Widget& what,
                Widget& to,
                GUI_::Anchor anchor,
                short axis
        );
            
        template< typename T = Vector2i >
        auto _get_axis( T t, int i ){
            return i == 0 ? t.x() : t.y();
        };

        template< typename T = Vector2i >
        T _get_rel_to_axis( T v1, T v2 , int axis ){
            return (axis == 0) ?
                T(
                    _get_axis( v1, axis ),
                    _get_axis( v2, !axis ) ) :
                T(
                    _get_axis( v2, !axis ),
                    _get_axis( v1, axis ) );
        };
        
        std::shared_ptr< Screen > screen;
        std::shared_ptr< FormHelper > helper;
    bool may_render = false; // dono if it'll be necesary.
};

extern GUI_ GUI;

class InputScreen : public nanogui::Screen {
    public:
        InputScreen();
        virtual ~InputScreen() = default;
        bool keyboardEvent(int key, int scancode, int action, int modifiers) override;
        void resize( const nanogui::Vector2i v );
};

class DefaultTheme : public Theme{
    public:
        DefaultTheme( NVGcontext *ctx );
        ~DefaultTheme();
};

#endif //!_MAIN_GUI_
