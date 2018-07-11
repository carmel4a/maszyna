/*
    This Source Code Form is subject to the
    terms of the Mozilla Public License, v.
    2.0. If a copy of the MPL was not
    distributed with this file, You can
    obtain one at
    http://mozilla.org/MPL/2.0/.
*/

#include "CustomWidget.h"
#include "nanogui/nanogui.h"
#include "GUI.h"
#include "Globals.h"
#include "Logs.h"

using namespace nanogui;
RootUI::RootUI()
        : CustomWidget( "root", std::shared_ptr<RootUI>(nullptr) ){
    
    widget_ = new Widget( GUI.screen() );
    widget_->setVisible(true);
};

void RootUI::update_tree(){

    for( auto const& x : widgets ){
        if( x.second->may_update ){
            x.second->update();
        }
    }
}
void RootUI::resize_tree( Vector2i v ){

    resize( v );
    for( auto const& x : widgets ){
        x.second->resize( v );
    }
};
void RootUI::delete_(){

    for( auto & x: widgets ){
        x.second = nullptr;
        widgets.erase( x.first );
    }
};
void RootUI::make(){

    show();
    resize( Vector2i( Global.iWindowWidth, Global.iWindowHeight ) );
};
