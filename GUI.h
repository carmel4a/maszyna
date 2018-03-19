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

#include "GL/glew.h"
#include "nanogui/nanogui.h"
using namespace nanogui;

class GUI_{
    public:
    GUI_();
    ~GUI_();

    void draw_gui();
    void init(GLFWwindow* window);
    Screen *screen;
    private:
    bool is_ready = false;
};

extern GUI_ GUI;

#endif //!_MAIN_GUI_
