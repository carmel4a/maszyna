/**docs'll go here*/
/*
This Source Code Form is subject to the
terms of the Mozilla Public License, v.
2.0. If a copy of the MPL was not
distributed with this file, You can
obtain one at
http://mozilla.org/MPL/2.0/.
*/
#ifndef GUI_MS_INCLUDED
    #define GUI_MS_INCLUDED

    #include "nuklear.h"

    class GUI_{
    public:
        GUI_();
        ~GUI_();

        void start_loop();
        void end_loop(GLFWwindow* window);

        void draw(GLFWwindow* window);

        void input_key(int key, int scancode, int action, int mods);
        void input_mouse_button(GLFWwindow* window, int button, int action, int mods);
        void input_mouse_motion(double x, double y);
        void input_scroll(double xoffset, double yoffset);

    protected:
        struct nk_context *ctx_ptr; // gui state
        struct nk_font_atlas *font_atlas_ptr;
        struct nk_user_font *u_font_ptr;

    private:
        void prepare_font_handler(struct nk_font*);
    };

#endif // !GUI_MS_INCLUDED
