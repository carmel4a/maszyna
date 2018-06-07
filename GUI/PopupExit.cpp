/*
    This Source Code Form is subject to the
    terms of the Mozilla Public License, v.
    2.0. If a copy of the MPL was not
    distributed with this file, You can
    obtain one at
    http://mozilla.org/MPL/2.0/.
*/

#include <vector>
#include <functional>
#include <memory>

#include "GUI.h"
#include "CustomWidget.h"
#include "PopupExit.h"
#include "Globals.h"

using namespace nanogui;
class GUI_; class CustomWidget; class PopupExitPanel;

void assign_grid_layout(Widget* to, const nanogui::Orientation orientation, const std::vector< Alignment > alignment);

PopupExit::PopupExit(){};

PopupExit::~PopupExit(){};

void PopupExit::init(){

    widget = new PopupExitPanel( GUI.get_screen(), shared_from_this() );
};

void PopupExit::init_layout(){

    const std::vector< Alignment > alignment_vector {
        nanogui::Alignment::Minimum,
        nanogui::Alignment::Middle
    };
    assign_grid_layout(
            widget.get(),
            nanogui::Orientation::Vertical,
            alignment_vector );
};

void PopupExit::make(){

    nanogui::ref< Label > text = new Label( widget.get(), "Exit Maszyna?" );
    nanogui::ref< Widget > buttons_group = new Widget( widget.get() );
    const std::vector< Alignment > alignment_vector {
        nanogui::Alignment::Minimum,
        nanogui::Alignment::Minimum
    };
    assign_grid_layout(
            buttons_group.get(),
            nanogui::Orientation::Horizontal,
            alignment_vector );

    nanogui::ref< Button > button_yes = new Button( buttons_group, "[Y]es" );
    button_yes->setCallback(
        std::bind( &PopupExit::exit, this )
    );
    nanogui::ref< Button > button_no = new Button( buttons_group, "[N]o" );
    button_no->setCallback(
        std::bind( &PopupExit::no_exit, this )
    );

    GUI.update_layout(buttons_group.get());
    GUI.update_layout(widget.get());
    
    auto anchor = GUI_::Anchor( GUI.Alignment::Centered );
    GUI.set_x_anchor(
        widget.get(),
        GUI.get_screen(),
        anchor
    );
    anchor.mode = GUI.Alignment::Begin;
    anchor.is_margin_rel = false;
    anchor.margin = 10;
    GUI.set_y_anchor(
        widget.get(),
        GUI.get_screen(),
        anchor
    );
};

void assign_grid_layout(                       Widget* to,
                            const nanogui::Orientation orientation,
                        const std::vector< Alignment > alignment    ){
    
    nanogui::ref< GridLayout > layout = new GridLayout(
            orientation,
            alignment.size(),                 // resolution 
            nanogui::Alignment::Middle,
            5,                                // margin
            5);                               // spacing
    to->setLayout( layout.get() );
    dynamic_cast< GridLayout* >( to->layout() )->setColAlignment( alignment );
};

void PopupExit::show(){

    widget->setVisible( true );
    widget->requestFocus();
    GUI.update_layout( widget.get() );
};

void PopupExit::hide(){
    
    widget->setVisible( false );
    GUI.get_screen()->requestFocus();
    GUI.update_layout( widget.get() );
};

void PopupExit::exit(){

    glfwSetWindowShouldClose( GUI.get_screen()->glfwWindow(), 1 );
};
void PopupExit::no_exit(){

    hide();
};

PopupExitPanel::PopupExitPanel( Widget* w, std::shared_ptr< PopupExit > _r ):
        Window( w, "" ),
        root{_r}{
    
    setVisible( false );
};

PopupExitPanel::~PopupExitPanel(){};

bool PopupExitPanel::keyboardEvent( int key, int scancode, int action, int modifiers ){

    if( key == GLFW_KEY_T || key == GLFW_KEY_Y ){
        root->exit();
        return true;
    }
    if( key == GLFW_KEY_N ){
        root->no_exit();
        return true;
    }
    return false;
};
