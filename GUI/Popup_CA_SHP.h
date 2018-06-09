/*
    This Source Code Form is subject to the
    terms of the Mozilla Public License, v.
    2.0. If a copy of the MPL was not
    distributed with this file, You can
    obtain one at
    http://mozilla.org/MPL/2.0/.
*/
#pragma once

#ifndef _POPUP_CA_SHP_
#define _POPUP_CA_SHP_

#include "CustomWidget.h"

class Popup_CA_SHP:
        public CustomWidget{
    public:
        Popup_CA_SHP();
        virtual ~Popup_CA_SHP();
        void init() override;
        void make() override;
        
        void update() override;
        void show() override;
        void hide() override;
        void resize( Vector2i v );
    private:
        nanogui::ref< Label > widget;
};

#endif /* !_POPUP_CA_SHP_ */
