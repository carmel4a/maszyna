/*
    This Source Code Form is subject to the
    terms of the Mozilla Public License, v.
    2.0. If a copy of the MPL was not
    distributed with this file, You can
    obtain one at
    http://mozilla.org/MPL/2.0/.
*/

#include <memory>

#include "CustomWidget.h"
#include "GUI.h"
#ifndef NDEBUG
    #include "Logs.h"
#endif // ! NDEBUG

CustomWidget::CustomWidget(
        std::string Name,
        shared_c_widget Owner
    ) : name{Name}
      , owner{Owner}
      , may_update{false} {};

CustomWidget::~CustomWidget(){

    Widget * parent = widget_->parent();
    if( dynamic_cast<Widget*>(parent) ){
        parent->removeChild( widget_.get() );
        GUI.update_layout( parent );
    }
    // eg. if it is root
    if ( owner ) owner->erase_child( name );
    #ifndef NDEBUG
        WriteLog( "CustomWidget " + name + " deleted." );
    #endif // ! NDEBUG
};

/*
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
*/

void CustomWidget::init(){

    set_owner( owner );
    make();
    GUI.update_layout( widget_.get() );
    #ifndef NDEBUG
        WriteLog("CustomWidget" + name + " created.");
    #endif // ! NDEBUG
};

CustomWidget * CustomWidget::get_child( std::string widget_name )
{
    try{
        return widgets.at(widget_name).get();
    } catch (const std::out_of_range& oor) {
        std::cerr << "Out of Range error: " << oor.what() << '\n' << "No"
        "widget named " << widget_name << " is registered in " << name << '\n';
        throw;
    }
};

void CustomWidget::set_owner( shared_c_widget sp_cw ){

    if( owner ) owner->erase_child( name );
    if( ! sp_cw ){
        owner = nullptr;
    } else {
        owner = sp_cw;
        owner->add_child( shared_from_this() );
    }
};

void CustomWidget::resize( Vector2i v ){};

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
