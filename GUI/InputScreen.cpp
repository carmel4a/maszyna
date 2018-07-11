/*
    This Source Code Form is subject to the
    terms of the Mozilla Public License, v.
    2.0. If a copy of the MPL was not
    distributed with this file, You can
    obtain one at
    http://mozilla.org/MPL/2.0/.
*/

#include "nanogui/nanogui.h"
#include "InputScreen.h"
#include "GUI.h"

using namespace nanogui;

/////////////////
// InputScreen //
/////////////////

InputScreen::InputScreen(){

    // Nanogui stuff. Necessary to get resize event.
    setResizeCallback(
        [this]( Vector2i v )->void{
            if((v.x() > 0) && (v.y() > 0))
                resize( v );
        }
    );
    /* not working alternative. Propably better than
       lambda (doesn't create unnamed class).
       setResizeCallback( std::bind( &InputScreen::resize(), this, std::placeholders::_1 ) ); */
};

InputScreen::~InputScreen(){};

bool InputScreen::keyboardEvent( int key, int scancode, int action, int modifiers ){

    if( GUI.root->keyboardEvent( key, scancode, action, modifiers )  ) return true;
    if( propagate_key( key, scancode, action, modifiers )  ) return true;
    return false;
};
void InputScreen::resize( nanogui::Vector2i v ){
    if(GUI.root) GUI.root->resize_tree( v );
};

bool InputScreen::propagate_key( int key, int scancode, int action, int modifiers ){

    if (mFocusPath.size() > 0) {
        for (auto it = mFocusPath.rbegin() + 1; it != mFocusPath.rend(); ++it)
            if ((*it)->focused() && (*it)->keyboardEvent(key, scancode, action, modifiers))
                return true;
    }
    return false;        
};
