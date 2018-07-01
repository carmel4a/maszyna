/*
    This Source Code Form is subject to the
    terms of the Mozilla Public License, v.
    2.0. If a copy of the MPL was not
    distributed with this file, You can
    obtain one at
    http://mozilla.org/MPL/2.0/.
*/

#ifndef _UI_SIMULATION_
#define _UI_SIMULATION_

//#include "RootUI.h"
#include "Yoga.h"
#include "nanogui/nanogui.h"
#include "RootUI.h"
using namespace nanogui;

class UI_Simulation:
        public RootUI{
  public:
    UI_Simulation();
    virtual ~UI_Simulation() = default;

    void resize( Vector2i v ) override;
    bool keyboardEvent(int key, int scancode, int action, int modifiers) override;
};

#endif /* !_UI_SIMULATION_ */