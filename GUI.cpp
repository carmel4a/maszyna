/**docs'll go here*/
/*
This Source Code Form is subject to the
terms of the Mozilla Public License, v.
2.0. If a copy of the MPL was not
distributed with this file, You can
obtain one at
http://mozilla.org/MPL/2.0/.
*/


#ifndef NUKLEAR_DEFS
    #define NUKLEAR_DEFS

    #define NK_IMPLEMENTATION

    #define NK_INCLUDE_STANDARD_IO

    #define NK_ASSERT

    #define NK_INCLUDE_DEFAULT_ALLOCATOR

#endif

#include "nuklear.h"
#include "GUI.hpp"
#include <GLFW/glfw3.h>

GUI_::GUI_(){

    nk_init_default(&this->ctx, 0);
};

GUI_::~GUI_(){
    
    nk_free(&this->ctx);
};

void GUI_::start_loop(){

    nk_input_begin(&this->ctx);
};

void GUI_::end_loop(){

    nk_input_end(&this->ctx);
    nk_clear(&this->ctx);
};

void GUI_::input_mouse_motion(double x, double y){;

    nk_input_motion(&this->ctx, (int)std::round(x), (int)std::round(y));
};

void GUI_::input_key(int key, int scancode, int action, int mods){
    int action_;
    nk_keys key_;
    // 0 - released, 1 - pressed, 2 - echo
    if (action != GLFW_REPEAT){
        action_ = action;
    }
    else{
        return;
    }
    switch (key)
    {
        case GLFW_KEY_UNKNOWN: key_ = NK_KEY_NONE; break;
        case GLFW_KEY_SPACE: break;
        case GLFW_KEY_APOSTROPHE: break;
        case GLFW_KEY_COMMA: break;
        case GLFW_KEY_MINUS: break;
        case GLFW_KEY_PERIOD: break;
        case GLFW_KEY_SLASH: break;
        case GLFW_KEY_0: break;
        case GLFW_KEY_1: break;
        case GLFW_KEY_2: break;
        case GLFW_KEY_3: break;
        case GLFW_KEY_4: break;
        case GLFW_KEY_5: break;
        case GLFW_KEY_6: break;
        case GLFW_KEY_7: break;
        case GLFW_KEY_8: break;
        case GLFW_KEY_9: break;
        case GLFW_KEY_SEMICOLON: break;
        case GLFW_KEY_EQUAL: break;
        case GLFW_KEY_A: break;
        case GLFW_KEY_B: break;
        case GLFW_KEY_C:
            if(mods & GLFW_MOD_CONTROL == GLFW_MOD_CONTROL){
                 key_ = NK_KEY_COPY;
            }
            else{
                //C
                break;
            }
            break;
        case GLFW_KEY_D: break;
        case GLFW_KEY_E: break;
        case GLFW_KEY_F: break;
        case GLFW_KEY_G: break;
        case GLFW_KEY_H: break;
        case GLFW_KEY_I: break;
        case GLFW_KEY_J: break;
        case GLFW_KEY_K: break;
        case GLFW_KEY_L: break;
        case GLFW_KEY_M: break;
        case GLFW_KEY_N: break;
        case GLFW_KEY_O: break;
        case GLFW_KEY_P: break;
        case GLFW_KEY_Q: break;
        case GLFW_KEY_R: break;
        case GLFW_KEY_S: break;
        case GLFW_KEY_T: break;
        case GLFW_KEY_U: break;
        case GLFW_KEY_V: break;
            if(mods & GLFW_MOD_CONTROL == GLFW_MOD_CONTROL){
                 key_ = NK_KEY_PASTE;
            }
            else{
                //V
                break;
            }
            break;
        case GLFW_KEY_W: break;
        case GLFW_KEY_X: break;
            if(mods & GLFW_MOD_CONTROL == GLFW_MOD_CONTROL){
                 key_ = NK_KEY_CUT;
            }
            else{
                //X
                break;
            }
            break;
        case GLFW_KEY_Y: break;
        case GLFW_KEY_Z: break;
        case GLFW_KEY_LEFT_BRACKET: break;
        case GLFW_KEY_BACKSLASH: break;
        case GLFW_KEY_RIGHT_BRACKET: break;
        case GLFW_KEY_GRAVE_ACCENT: break;
        case GLFW_KEY_WORLD_1: break;
        case GLFW_KEY_WORLD_2: break;
        case GLFW_KEY_ESCAPE: break;
        case GLFW_KEY_ENTER: key_ = NK_KEY_ENTER; break;
        case GLFW_KEY_TAB: key_ = NK_KEY_TAB; break;
        case GLFW_KEY_BACKSPACE: key_ = NK_KEY_BACKSPACE; break;
        case GLFW_KEY_INSERT: break;
        case GLFW_KEY_DELETE: key_ = NK_KEY_DEL; break;
        case GLFW_KEY_RIGHT: key_ = NK_KEY_RIGHT; break;
        case GLFW_KEY_LEFT: key_ = NK_KEY_LEFT; break;
        case GLFW_KEY_DOWN: key_ = NK_KEY_DOWN; break;
        case GLFW_KEY_UP: key_ = NK_KEY_UP; break;
        case GLFW_KEY_PAGE_UP: break;
        case GLFW_KEY_PAGE_DOWN: break;
        case GLFW_KEY_HOME: break;
        case GLFW_KEY_END: break;
        case GLFW_KEY_CAPS_LOCK: break;
        case GLFW_KEY_SCROLL_LOCK: break;
        case GLFW_KEY_NUM_LOCK: break;
        case GLFW_KEY_PRINT_SCREEN: break;
        case GLFW_KEY_PAUSE: break;
        case GLFW_KEY_F1: break;
        case GLFW_KEY_F2: break;
        case GLFW_KEY_F3: break;
        case GLFW_KEY_F4: break;
        case GLFW_KEY_F5: break;
        case GLFW_KEY_F6: break;
        case GLFW_KEY_F7: break;
        case GLFW_KEY_F8: break;
        case GLFW_KEY_F9: break;
        case GLFW_KEY_F10: break;
        case GLFW_KEY_F11: break;
        case GLFW_KEY_F12: break;
        case GLFW_KEY_F13: break;
        case GLFW_KEY_F14: break;
        case GLFW_KEY_F15: break;
        case GLFW_KEY_F16: break;
        case GLFW_KEY_F17: break;
        case GLFW_KEY_F18: break;
        case GLFW_KEY_F19: break;
        case GLFW_KEY_F20: break;
        case GLFW_KEY_F21: break;
        case GLFW_KEY_F22: break;
        case GLFW_KEY_F23: break;
        case GLFW_KEY_F24: break;
        case GLFW_KEY_F25: break;
        case GLFW_KEY_KP_0: break;
        case GLFW_KEY_KP_1: break;
        case GLFW_KEY_KP_2: break;
        case GLFW_KEY_KP_3: break;
        case GLFW_KEY_KP_4: break;
        case GLFW_KEY_KP_5: break;
        case GLFW_KEY_KP_6: break;
        case GLFW_KEY_KP_7: break;
        case GLFW_KEY_KP_8: break;
        case GLFW_KEY_KP_9: break;
        case GLFW_KEY_KP_DECIMAL: break;
        case GLFW_KEY_KP_DIVIDE: break;
        case GLFW_KEY_KP_MULTIPLY: break;
        case GLFW_KEY_KP_SUBTRACT: break;
        case GLFW_KEY_KP_ADD: break;
        case GLFW_KEY_KP_ENTER: key_ = NK_KEY_ENTER; break;
        case GLFW_KEY_KP_EQUAL: break;
        case GLFW_KEY_LEFT_SHIFT: key_ = NK_KEY_SHIFT; break;
        case GLFW_KEY_LEFT_CONTROL: key_ = NK_KEY_CTRL; break;
        case GLFW_KEY_LEFT_ALT: break;
        case GLFW_KEY_LEFT_SUPER: break;
        case GLFW_KEY_RIGHT_SHIFT: key_ = NK_KEY_SHIFT; break;
        case GLFW_KEY_RIGHT_CONTROL: key_ = NK_KEY_CTRL; break;
        case GLFW_KEY_RIGHT_ALT: break;
        case GLFW_KEY_RIGHT_SUPER: break;
        case GLFW_KEY_MENU: break;
        default:
            break;
    }
    nk_input_key(&this->ctx, key_, action_);
};

void GUI_::input_mouse_button(GLFWwindow* window, int button, int action, int mods){

    int action_;
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    nk_buttons button_;
    // 0 - released, 1 - pressed
    action_ = action;
    switch (button)
    {
        case GLFW_MOUSE_BUTTON_LEFT: button_ = NK_BUTTON_LEFT; break;
        case GLFW_MOUSE_BUTTON_MIDDLE: button_ = NK_BUTTON_MIDDLE; break;
        case GLFW_MOUSE_BUTTON_RIGHT: button_ = NK_BUTTON_RIGHT; break;
        default:
            break;
    }
    nk_input_button(&this->ctx, button_, (int)std::round(x), (int)std::round(y), action_);
};

void GUI_::input_scroll(double xoffset, double yoffset){

    struct nk_vec2 v;
    v.x = (float)xoffset;
    v.y = (float)yoffset;
    nk_input_scroll(&this->ctx, v);
}