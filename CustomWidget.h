/*
    This Source Code Form is subject to the
    terms of the Mozilla Public License, v.
    2.0. If a copy of the MPL was not
    distributed with this file, You can
    obtain one at
    http://mozilla.org/MPL/2.0/.
*/
#pragma once

#ifndef _CUSTOM_WIDGET_
#define _CUSTOM_WIDGET_

#include "GUI.h"
#include "nanogui/nanogui.h"
#include <string>

using namespace nanogui;
class GUI_;
class CustomWidget;

typedef std::shared_ptr< CustomWidget > shared_custom_widget;
typedef std::unordered_map< std::string, shared_custom_widget > widget_map;

class CustomWidget{

    public:
        CustomWidget(){};
        virtual ~CustomWidget(){};
        Widget* get_widget(){ return widget.get(); };
        nanogui::ref<Widget> get_widget_ref(){ return widget; };
        virtual void init_layout() = 0;
        virtual void make() = 0;
        widget_map widgets;
    
    protected:
        nanogui::ref<Widget> widget;
};

class TestWidget : public CustomWidget{
    
    public:
        TestWidget( std::string text, nanogui::ref<Widget> parent );
        ~TestWidget(){};
        virtual void init_layout();
        virtual void make();
        
        nanogui::ref<Widget> parent;
        std::string test = "aaa";
};

#endif // !_CUSTOM_WIDGET_
