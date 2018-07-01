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

void RootUI::init(){

    widget_ = new Widget( GUI.screen() );
    show();
};

void RootUI::make(){

    show();
    resize( Vector2i( Global.iWindowWidth, Global.iWindowHeight ) );
};
 
void RootUI::resize( Vector2i v ){

    YGNodeStyleSetWidth( YG_node, v.x() );
    YGNodeStyleSetHeight( YG_node, v.y() );
    YGNodeStyleSetMinWidth( YG_node, v.x() );
    YGNodeStyleSetMinHeight( YG_node, v.y() );
    YGNodeStyleSetPositionType( YG_node, YGPositionTypeAbsolute );
    
    calc_layout( v );
};

