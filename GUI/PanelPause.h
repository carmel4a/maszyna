/*
    This Source Code Form is subject to the
    terms of the Mozilla Public License, v.
    2.0. If a copy of the MPL was not
    distributed with this file, You can
    obtain one at
    http://mozilla.org/MPL/2.0/.
*/
#pragma once

#ifndef _POPUP_PAUSE_
#define _POPUP_PAUSE_

#include "CustomWidget.h"

class PopupExitPanel;

class PanelPause:
        public CustomWidget{
    public:
        PanelPause();
        virtual ~PanelPause();
        void init() override;
        void make() override;
        
        void update() override;
        void show() override;
        void hide() override;
        void resize( Vector2i v );
    private:
        nanogui::ref< Window > panel;
};

#endif /* !_POPUP_PAUSE_ */