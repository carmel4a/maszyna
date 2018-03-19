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

#include "GL/glew.h"
#include "nanogui/nanogui.h"
#include "CustomWidget.h"

using namespace nanogui;

class CustomWidget;

typedef std::shared_ptr< CustomWidget > shared_custom_widget;
typedef std::unordered_map< std::string, shared_custom_widget > widget_map;

class GUI_{

    public:
        GUI_();
        ~GUI_();

        /*
            //Vector2i to_glob_pos(Vector2f rel_pos); // TODO
            //Vector2f to_rel_pos(Vector2f glob_pos); // TODO
        */
        
        /*! Call after succesfull init of glfw window. */
        void init(GLFWwindow* window);
        
        /*! Call it every frame. */
        void draw_gui();
        
        void update_all_vars();
        
        /*! nanogui::Screen class, handles GLFW window. Represent an glfw window.
        It should be an singleton, untill MaSzyna will have one main window.
        It may change eg. when mp module will be implemented.*/
        Screen* get_screen();
        FormHelper* get_helper();

        void add_widget(                 std::string name,
                        const shared_custom_widget& sptr_custom,
                                        widget_map& where        );
        
        void remove_widget( std::string name,
                                 widget_map& from  );
        
        widget_map& get_widgets_map( CustomWidget& widget ); // not implemented // outdated
        void update_layout( Widget* of );
        void focus_helper_on( Window& window );
    
    private:
        std::shared_ptr< Screen > screen;
        std::shared_ptr< FormHelper > helper;
        widget_map widgets;
        bool is_ready = false; // dono if it'll be necesary.
};

extern GUI_ GUI;

#endif //!_MAIN_GUI_
