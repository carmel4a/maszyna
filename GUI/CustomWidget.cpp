/*
    This Source Code Form is subject to the
    terms of the Mozilla Public License, v.
    2.0. If a copy of the MPL was not
    distributed with this file, You can
    obtain one at
    http://mozilla.org/MPL/2.0/.
*/

#include "CustomWidget.h"
#include "GUI.h"

CustomWidget::CustomWidget()
        : may_update{false} {
};

CustomWidget::CustomWidget( const CustomWidget& x ){

    this->may_update = x.may_update;
    this->owner = x.owner; // shallow copy is ok here, as owner is const
    for( const auto & x : x.widgets ){
        this->widgets[x.first] = std::shared_ptr<CustomWidget>( x.second->clone() );
    }
    this->widget_ = nanogui::ref<Widget>( new Widget( *( x.widget_.get() ) ) );
};

CustomWidget& CustomWidget::operator= ( const CustomWidget& x ){};
CustomWidget::CustomWidget( CustomWidget&& x ){};
CustomWidget& CustomWidget::operator= ( CustomWidget&& x ){};

void CustomWidget::resize( Vector2i v ){

};

void CustomWidget::show(){

    widget_->setVisible( true );
    GUI.update_layout( widget_.get() );
    may_update = true;
};

void CustomWidget::hide(){

    widget_->setVisible( false );
    GUI.update_layout( widget_.get() );
    widget_->setFocused( false );
    may_update = false;
};

CustomWidget::operator std::string(){
    
    for( auto const& x : *owner ) {
        if( x.second.get() == this ) return( x.first );
    }
    std::string s = "No *CustomWidget fund in owner!";
    throw s;
};
