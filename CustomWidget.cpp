/*
    This Source Code Form is subject to the
    terms of the Mozilla Public License, v.
    2.0. If a copy of the MPL was not
    distributed with this file, You can
    obtain one at
    http://mozilla.org/MPL/2.0/.
*/

#include <memory.h>
#include <vector>

#include "CustomWidget.h"
#include "Logs.h"
#include "Globals.h"

using namespace nanogui;

TestWidget::TestWidget( std::string t, nanogui::ref<Widget> p ) :

    test{t},
    parent{p}{
};

void TestWidget::init_layout(){
    
    nanogui::ref<Layout> layout = new BoxLayout( Orientation::Horizontal );
    widget = nanogui::ref<Widget>( new Window( parent.get() ) );
    widget->setLayout( layout );
    widget->setPosition( Eigen::Vector2i( 500, 500 ) );
    widget->setVisible(true);
};

void TestWidget::make(){
    
    nanogui::ref<Label> _label = new Label( widget, "a test label" );
    _label->setCaption( test );
};
