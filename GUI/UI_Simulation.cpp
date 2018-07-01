/*
    This Source Code Form is subject to the
    terms of the Mozilla Public License, v.
    2.0. If a copy of the MPL was not
    distributed with this file, You can
    obtain one at
    http://mozilla.org/MPL/2.0/.
*/

#include "Yoga.h"
//#include "RootUI.h"
#include "nanogui/nanogui.h"
#include "UI_Simulation.h"
#include "GUI.h"
#include "PopupExit.h"
#include "LabelArray.h"

using namespace nanogui;

UI_Simulation::UI_Simulation(){

    widget_ = new Widget( GUI.screen() );
};

void UI_Simulation::resize( Vector2i v ){

    YGNodeStyleSetWidth( YG_node, v.x() );
    YGNodeStyleSetHeight( YG_node, v.y() );
    YGNodeStyleSetMinWidth( YG_node, v.x() );
    YGNodeStyleSetMinHeight( YG_node, v.y() );
    YGNodeStyleSetPositionType( YG_node, YGPositionTypeAbsolute );
    
    calc_layout( v );
};

bool UI_Simulation::keyboardEvent(int key, int scancode, int action, int modifiers){;

    if( key == GLFW_KEY_F10 ){
        const auto& popup = dynamic_cast< PopupExit* >( GUI.widgets["exit_popup"].get() );
        if( popup->may_quit && action == GLFW_PRESS ){
            if( popup->widget()->visible() ){
                popup->hide();
            }
            else{
                popup->show();
                popup->widget()->setFocused( true );
                popup->widget()->requestFocus();
            }
            return true;;
        }
    } else if( key == GLFW_KEY_F12 ){
        const auto& _log = dynamic_cast< LabelArray* >( GUI.widgets["loading_log"].get() );
        if( action == GLFW_PRESS ){
            if( _log->widget()->visible() ){
                _log->hide();
            }
            else{
                _log->show();
            }
            return true;
        }
    }
    return false;
};