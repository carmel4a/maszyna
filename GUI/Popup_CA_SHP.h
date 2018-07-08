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
        // Popup_CA_SHP( const Popup_CA_SHP & x ) {} // TODO;           // TODO
        // Popup_CA_SHP* create() const { return new Popup_CA_SHP(); }; // TODO
        Popup_CA_SHP* clone() const override { return new Popup_CA_SHP( *this ); };
        
        void init() override;
        void make() override;
        
        void update() override;
        void show() override;
        void hide() override;
        void resize( Vector2i v ) override;
    private:
        void show_CA();
        void hide_CA();
        void show_SHP();
        void hide_SHP();
        nanogui::ref< Label > ca;
        nanogui::ref< Label > shp;
};

#endif /* !_POPUP_CA_SHP_ */
