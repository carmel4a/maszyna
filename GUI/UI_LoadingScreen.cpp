/*
    This Source Code Form is subject to the
    terms of the Mozilla Public License, v.
    2.0. If a copy of the MPL was not
    distributed with this file, You can
    obtain one at
    http://mozilla.org/MPL/2.0/.
*/

#include "nanogui/nanogui.h"
#include "UI_LoadingScreen.h"
#include "GUI.h"
#include "LabelArray.h"
#include "Logs.h"
#include "Globals.h"

using namespace nanogui;

UI_LoadingScreen::UI_LoadingScreen(){

};

UI_LoadingScreen::~UI_LoadingScreen(){};

void UI_LoadingScreen::resize( Vector2i v ){

    widget_->setFixedSize( v );
};

bool UI_LoadingScreen::keyboardEvent(int key, int scancode, int action, int modifiers){;

    return false;
};