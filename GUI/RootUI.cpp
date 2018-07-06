/*
    This Source Code Form is subject to the
    terms of the Mozilla Public License, v.
    2.0. If a copy of the MPL was not
    distributed with this file, You can
    obtain one at
    http://mozilla.org/MPL/2.0/.
*/

#include "Yoga.h"
#include "CustomWidget.h"
#include "nanogui/nanogui.h"
#include "GUI.h"
#include "Globals.h"
#include "Logs.h"

using namespace nanogui;
RootUI::RootUI(){
    
    widget_ = new Widget( GUI.screen() );
    widget_->setVisible(true);
};
void RootUI::init(){

    show();
};

void RootUI::make(){

    show();
    resize( Vector2i( Global.iWindowWidth, Global.iWindowHeight ) );
};
