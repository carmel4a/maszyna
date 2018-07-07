/*
    This Source Code Form is subject to the
    terms of the Mozilla Public License, v.
    2.0. If a copy of the MPL was not
    distributed with this file, You can
    obtain one at
    http://mozilla.org/MPL/2.0/.
*/

#ifndef _UI_LOADING_SCREEN_
#define _UI_LOADING_SCREEN_

#include "nanogui/nanogui.h"
#include "RootUI.h"

using namespace nanogui;

class UI_LoadingScreen:
        public RootUI{
  public:
    UI_LoadingScreen();
    virtual ~UI_LoadingScreen();

    void resize( Vector2i v ) override;
    bool keyboardEvent(int key, int scancode, int action, int modifiers) override;
    ref<Widget> top_widget;
    YGNodeRef top;
    ref<Widget> test;
    YGNodeRef YG_test;
};

#endif /* !_UI_LOADING_SCREEN_ */