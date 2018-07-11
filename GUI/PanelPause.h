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
        PanelPause(
                std::string Name,
                shared_c_widget Owner );
        virtual ~PanelPause() = default;
        // PanelPause( const PanelPause & x ) {}                    // TODO;
        // PanelPause* create() const { return new PanelPause(); }; // TODO
        PanelPause* clone() const override { return new PanelPause( *this ); };

        void make() override;
        
        void resize( Vector2i v ) override;
        void hide() override;
        void update() override;
    private:
        nanogui::ref< Window > panel;
};

#endif /* !_POPUP_PAUSE_ */
