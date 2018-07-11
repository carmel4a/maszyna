/*
    This Source Code Form is subject to the
    terms of the Mozilla Public License, v.
    2.0. If a copy of the MPL was not
    distributed with this file, You can
    obtain one at
    http://mozilla.org/MPL/2.0/.
*/

#pragma once

#ifndef _INPUT_SCREEN_
#define _INPUT_SCREEN_

#include "nanogui/nanogui.h"
#include "RootUI.h"

using namespace nanogui;

/// Derivered screen class, to implement own logic.
/** Handles own YGNode, propagate inputs. */
class InputScreen : public nanogui::Screen {
  public:
    InputScreen();
    virtual ~InputScreen();

    bool keyboardEvent( int key, int scancode, int action, int modifiers ) override;

    /** It do NOT resize `Widgets`, it only call `resize()` on CustomWidgets. If
     *  there is need to do sth on every children, it must be implemented.
     */
    void resize( nanogui::Vector2i v );
    
  protected:
    /** Default implementation from nanogui::Screen */
    bool propagate_key( int key, int scancode, int action, int modifiers );
};

#endif // !_INPUT_SCREEN_