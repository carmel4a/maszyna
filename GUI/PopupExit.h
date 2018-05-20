/*
    This Source Code Form is subject to the
    terms of the Mozilla Public License, v.
    2.0. If a copy of the MPL was not
    distributed with this file, You can
    obtain one at
    http://mozilla.org/MPL/2.0/.
*/
#pragma once

#ifndef _POPUP_EXIT_
#define _POPUP_EXIT_

#include <memory>

#include "CustomWidget.h"
#include "nanogui/nanogui.h"

class PopupExitPanel;

class PopupExit: 
        public CustomWidget, 
        public std::enable_shared_from_this< PopupExit >{
    friend PopupExitPanel;
    public:
        PopupExit();
        virtual ~PopupExit();
        void init();
        void init_layout();
        void make();
        
        void show();
        void hide();

        bool may_quit = true;

    private:
        void exit();
        void no_exit();
};

class PopupExitPanel : public Window {
    
    public:
        PopupExitPanel( Widget* widget, std::shared_ptr< PopupExit > _root );
        virtual ~PopupExitPanel();
        bool keyboardEvent(int key, int scancode, int action, int modifiers) override;

        std::shared_ptr< PopupExit > root;
};

#endif /* !_POPUP_EXIT_ */
