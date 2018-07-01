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
        : may_update{false}
        , YG_node{YGNodeNew()} {
    YGNodeStyleSetDirection( YG_node, YGDirectionLTR );
};

void CustomWidget::resize( Vector2i v ){

    calc_layout( v );
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

void CustomWidget::calc_layout( Vector2i v ){

    // Calculate layout
    YGNodeCalculateLayout( YG_node, (float) v.x(), (float) v.y(), YGDirectionLTR );
    // Set position
    Vector2i _temp = Vector2i( YGNodeLayoutGetLeft( YG_node ), YGNodeLayoutGetTop( YG_node ) );
    widget_->setPosition( _temp );
    // Set size
    _temp = Vector2i( YGNodeLayoutGetWidth( YG_node ), YGNodeLayoutGetHeight( YG_node ) );
    widget_->setSize( _temp );
};

CustomWidget::operator std::string(){
    
    for( auto const& x : *owner ) {
        if( x.second.get() == this ) return( x.first );
    }
    std::string s = "No *CustomWidget fund in owner!";
    throw s;
};
