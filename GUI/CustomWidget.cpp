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

// Widget after init WON'T autoupdate.
CustomWidget::CustomWidget(
        std::string Name,
        shared_c_widget Owner
    ) : name{Name}
      , owner{Owner}
      , may_update{false} {
    #ifndef NDEBUG
        WriteLog( "CustomWidget " + name + " created." );
    #endif // ! NDEBUG
};

// Hope that's all what we need to do...
CustomWidget::~CustomWidget(){

    // Remove nanogui::Widget from parent.
    auto* parent = dynamic_cast<Widget*>( widget_->parent() );
    if( parent ) {
        parent->removeChild( widget_.get() );
        GUI.update_layout( parent );
    }
    // Remove self from owner's widget_map.
    // "If it isn't root:" (roots haven't owner).
    if ( owner ) owner->erase_child( name );
    #ifndef NDEBUG
        WriteLog( "CustomWidget " + name + " deleted." );
    #endif // ! NDEBUG
};

// someone should implement that shit.
// GL with copying nanogui::Widget
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

    set_owner( owner ); // should work even if null.
    // User provided CustomWidget implementation.
    // From now we may assume, that widget_ is not null...
    make();
    GUI.update_layout( widget_.get() );
    #ifndef NDEBUG
        WriteLog("CustomWidget " + name + " init.");
    #endif // ! NDEBUG
};

// dono if it should throw exception, or not..?
CustomWidget* CustomWidget::get_child( std::string name )
{
    try{
        return widgets.at( name ).get();
    } catch (const std::out_of_range& oor) {
        return nullptr;
        #ifndef DNEBUG
            std::cerr << "Out of Range error: " << oor.what() << '\n' << "No"
            "widget named " << name << " is registered in " << name << '\n';
        #endif // ! DNEBUG
    }
};

void CustomWidget::set_owner( shared_c_widget sp_cw ){

    // Unregister from owner's widget_map, if present.
    if( owner ) owner->erase_child( name );
    if( ! sp_cw ){
        owner = nullptr;
    } else {
        owner = sp_cw;
        owner->add_child( shared_from_this() );
    }
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
