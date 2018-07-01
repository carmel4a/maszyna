/*
    This Source Code Form is subject to the
    terms of the Mozilla Public License, v.
    2.0. If a copy of the MPL was not
    distributed with this file, You can
    obtain one at
    http://mozilla.org/MPL/2.0/.
*/

#ifndef _ROOT_UI_
#define _ROOT_UI_

#include "CustomWidget.h"
#include "Yoga.h"
#include "nanogui/nanogui.h"


using namespace nanogui;

class CustomWidget;

class RootUI:
        public CustomWidget, 
        public std::enable_shared_from_this< RootUI >{
  public:
    RootUI() = default;
    virtual ~RootUI() = default;
    void init() override;
    void make() override;
    void resize( Vector2i v ) override;
    virtual bool keyboardEvent(int key, int scancode, int action, int modifiers) = 0;
};

#endif /* !_UI_SIMULATION_ */