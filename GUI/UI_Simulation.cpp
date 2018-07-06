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
#include "Globals.h"

using namespace nanogui;

UI_Simulation::UI_Simulation(){

};

UI_Simulation::~UI_Simulation(){

};

void UI_Simulation::resize( Vector2i v ){

    widget_->setFixedSize( v );
};

bool UI_Simulation::keyboardEvent(int key, int scancode, int action, int modifiers){;

    if( key == GLFW_KEY_F10 ){
        auto popup = GUI.root->widgets["exit_popup"];

        bool may_quit = dynamic_cast< PopupExit* >( popup.get() )->may_quit;
        bool pressed = action == GLFW_PRESS;
        if( may_quit && pressed ){
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

        auto _log = GUI.root->widgets["loading_log"];
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