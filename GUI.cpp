#include "GUI.h"
#include "Logs.h"

GUI_ GUI;

GUI_::GUI_(){;
};

GUI_::~GUI_(){
    delete this->screen;
};
void GUI_::init(GLFWwindow* window){
    // Create nanogui gui
    this->screen = new Screen();
    this->screen->initialize(window, true);
    bool enabled = true;

    this->screen->setVisible(true);
    this->screen->performLayout();
    this->is_ready = true;
}
void GUI_::draw_gui(){
    if (this->is_ready){
        this->screen->drawContents();
        this->screen->drawWidgets();
    }
};
