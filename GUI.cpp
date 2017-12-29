/**docs'll go here*/
/*
This Source Code Form is subject to the
terms of the Mozilla Public License, v.
2.0. If a copy of the MPL was not
distributed with this file, You can
obtain one at
http://mozilla.org/MPL/2.0/.
*/

//////////
// Defs //
//////////


#ifndef NUKLEAR_DEFS
    #define NUKLEAR_DEFS

    #define NK_IMPLEMENTATION
    // Include <stdint.h> Size of types.
    #define NK_INCLUDE_FIXED_TYPES
    // Include <stdlib.h> Easy memory management - more funcs.
    #define NK_INCLUDE_DEFAULT_ALLOCATOR
    // Standard io <stdio.h> More funcs for file loading.
    #define NK_INCLUDE_STANDARD_IO
    // Include <stdarg.h> More functions for variable arguments.
    #define NK_INCLUDE_STANDARD_VARARGS
    // Used for low level vertex hardware management (opengl).
    #define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
    // Font management.
    #define NK_INCLUDE_FONT_BAKING
    // Add ProggyClean.ttf.
    #define NK_INCLUDE_DEFAULT_FONT
    /*
    // ? shaders in widgets Don'no know what's that.
    #define NK_INCLUDE_COMMAND_USERDATA
    // ?change buttons behaviour
    #define NK_BUTTON_TRIGGER_ON_RELEASE
    */

#endif // !NUKLEAR_DEFS

//////////////
// Includes //
//////////////

#include "Globals.h"
#include "nuklear.h"
#include "GUI.hpp"
#include <GLFW/glfw3.h>
#include <string>
#include "Logs.h"

#include <iostream>

/////////////
// Members //
/////////////

GUI_::GUI_(){

    this->font_atlas_ptr = new struct nk_font_atlas;
    this->u_font_ptr = new struct nk_user_font;
    this->ctx_ptr = new struct nk_context;

    nk_font_atlas_init_default(this->font_atlas_ptr);
    nk_font_atlas_begin(this->font_atlas_ptr);
    nk_font *font = nk_font_atlas_add_from_file(this->font_atlas_ptr, "ref/fonts/cour.ttf", 13, 0);
    this->prepare_font_handler(font);
    //nk_font *font2 = nk_font_atlas_add_from_file(&atlas_, "Path/To/Your/TTF_Font2.ttf", 16, 0);
    int h = 100;
    int w = 100;
    const void *img = nk_font_atlas_bake(this->font_atlas_ptr, &h, &w, NK_FONT_ATLAS_RGBA32);
    //nk_font_atlas_end(&atlas, nk_handle_id(texture), 0);
    nk_font_atlas_end(this->font_atlas_ptr, nk_handle_id(NULL), 0);
    nk_init_default(this->ctx_ptr, this->u_font_ptr);
};

GUI_::~GUI_(){

    //nk_font_atlas_end()
    nk_font_atlas_cleanup(this->font_atlas_ptr);
    nk_font_atlas_clear(this->font_atlas_ptr);
    nk_free(this->ctx_ptr);
    delete this->font_atlas_ptr;
    delete this->u_font_ptr;
    delete this->ctx_ptr;
};

void GUI_::start_loop(){

    nk_input_begin(this->ctx_ptr);
};

void GUI_::end_loop(GLFWwindow* window){

    nk_input_end(this->ctx_ptr);
    this->draw(window);
    nk_clear(this->ctx_ptr);
};
float _text_width_calculation(nk_handle handle, float height, const char *text, int len){

    return 10;
    //your_font_type *type = handle.ptr;
    //float text_width = ...;
    //return text_width;        //your_font_type *type = handle.ptr;
    //float text_width = ...;
    //return text_width;
};
void GUI_::prepare_font_handler(struct nk_font* unno){

    //&this->font.userdata.ptr = &your_font_class_or_struct;
    //&this->font.height = your_font_height;
    //&this->font.width = your_text_width_calculation;
    this->u_font_ptr->userdata.ptr = unno;
    float w = 10;
    float h = 10;
    this->u_font_ptr->height = h;
    this->u_font_ptr->width = &_text_width_calculation;
};

void GUI_::draw(GLFWwindow* window){
    WriteLog("start_draw_ui");
    int x_, y_, w_, h_;
    struct nk_rect size;
    WriteLog("here");
    glfwGetWindowFrameSize(window, &x_, &y_, &w_, &h_);
    WriteLog("here");
    size.x = (float)0;
    size.y = (float)0;
    size.w = (float)w_;
    size.h = (float)h_;
    WriteLog("here");
    //WriteLog(Global::AppName + " | " +
    //std::to_string(size.x)+ " | "+std::to_string(size.y)+ " | "+
    //std::to_string(size.h)+ " | "+std::to_string(size.w)+ " | "+
    //std::to_string(nk_panel_flags::NK_WINDOW_BORDER & nk_panel_flags::NK_WINDOW_TITLE));
    //WriteLog(Global::AppName, true);
    WriteLog("EU07");
    nk_flags flags_ = nk_panel_flags::NK_WINDOW_BORDER | nk_panel_flags::NK_WINDOW_TITLE;
    char *name_ = "EU07";
    //Global::AppName.c_str()
    //int window_ =
    //        (int)nk_begin_titled(&this->ctx, name_,
    //        name_, size, flags_);
    int window_ = nk_begin(this->ctx_ptr, name_, size, flags_);
    //if (nk_begin(&this->ctx, name_, size, flags_)){
    WriteLog("here");
    if ((bool)window_){
    //if(nk_begin_titled(&this->ctx,
    //if(nk_window_find(&this->ctx, "EU07")){
        //[... widgets ...]
        WriteLog("window fund");
    }
    else{
        WriteLog("window not fund");
    }
    nk_end(this->ctx_ptr);

    const struct nk_command *cmd = 0;
    // That's a for loop... I <3 macros.
    nk_foreach(cmd, this->ctx_ptr){
        switch (cmd->type) {
            case NK_COMMAND_LINE:
                //your_draw_line_function(...)
                break;
            case NK_COMMAND_RECT:
                //your_draw_rect_function(...)
                break;
        }
    }
}

void GUI_::input_mouse_motion(double x, double y){

    nk_input_motion(this->ctx_ptr, (int)std::round(x), (int)std::round(y));
};

void GUI_::input_key(int key, int scancode, int action, int mods){
    int action_;
    nk_keys key_ = NK_KEY_NONE;
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
    if(key_ == NK_KEY_NONE){
        return;
    }
    nk_input_key(this->ctx_ptr, key_, action_);
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
            //break;
            return;
    }
    nk_input_button(this->ctx_ptr, button_, (int)std::round(x), (int)std::round(y), action_);
};

void GUI_::input_scroll(double xoffset, double yoffset){

    struct nk_vec2 v;
    v.x = (float)xoffset;
    v.y = (float)yoffset;
    nk_input_scroll(this->ctx_ptr, v);
}
