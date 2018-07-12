/*
    This Source Code Form is subject to the
    terms of the Mozilla Public License, v.
    2.0. If a copy of the MPL was not
    distributed with this file, You can
    obtain one at
    http://mozilla.org/MPL/2.0/.
*/
#pragma once

#ifndef _LABEL_ARRAY_WIDGET_
#define _LABEL_ARRAY_WIDGET_

#include "CustomWidget.h"
#include "tinyfsm.hpp"
#include <string> // std::string
#include <vector> // std::vector
#include <nanogui/nanogui.h>

using namespace nanogui;

class LabelArray:
        public CustomWidget{
  public:
    LabelArray(
            std::string name,
            shared_c_widget Owner,
            bool transparent = true,
            std::string HeaderName = "Label Array",
            int Size = 10,
            int FixedW = 500,
            int FixedH = -1,
            std::string Def_text = ""
    );
    virtual ~LabelArray();
    // LabelArray( const LabelArray & x ) {}                       // TODO;
    // LabelArray* create() const { return new LabelArray(); }; // TODO
    // LabelArray* clone() const override { return new LabelArray( *this ); };
    void make() override;
    
    void update() override;
    void show() override;
    void hide() override;
    void resize( Vector2i v ) override;

    int fixed_w;
    int fixed_h;
    int init_size;
    std::string header_name;
    std::string def_text;
    bool transparent;

    void push_line( std::string text );
    enum Mode : short{
        LIMITED,
        UNLIMITED
    };

    struct Data : public CustomWidget::Data {
    
        std::vector<std::string> labels;
    } data;

    void load( LabelArray::Data const * t );
    LabelArray::Data & save();

    std::bitset<2> mode;
  protected:
    ref<VScrollPanel> scroll_panel;
    ref<Widget> group;
    std::deque< ref<Label> > label_array;
    void pop(){};       // UNIMPLEMENTED
    void clear();
};

struct PrintLine : public tinyfsm::Event {
    PrintLine(std::string Text = "\n"): text{Text} {};
    virtual ~PrintLine(){};
    std::string text;
};

class LoadingLog:
    public LabelArray {
  public:
    LoadingLog(
        bool transparent = true,
        std::string Name = "Label Array",
        int Size = 10,
        int fixed_w = -1,
        std::string Def_text = ""
    );
    virtual ~LoadingLog();
    void init();
    void make();
        
    void update();
};

#endif /* !_LABEL_ARRAY_WIDGET_ */
