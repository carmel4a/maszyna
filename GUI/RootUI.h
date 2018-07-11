/*
    This Source Code Form is subject to the
    terms of the Mozilla Public License, v.
    2.0. If a copy of the MPL was not
    distributed with this file, You can
    obtain one at
    http://mozilla.org/MPL/2.0/.
*/

#ifndef _ROOT_UI_
#define _ROOT_UI_

#include "CustomWidget.h"
#include "nanogui/nanogui.h"

using namespace nanogui;

typedef std::shared_ptr< CustomWidget > shared_customwidget_ptr;
typedef std::unordered_map< std::string, shared_customwidget_ptr > widget_map;

class RootUI:
    public CustomWidget {
  public:
    RootUI();

    void update_tree();
    void resize_tree( Vector2i v );

    virtual ~RootUI() = default;

    void delete_();
    // virtual RootUI* create() const = 0; // TODO
    virtual RootUI* clone() const = 0;

    virtual void make() override;
    
    virtual void resize( Vector2i v ) = 0;
    virtual bool keyboardEvent(int key, int scancode, int action, int modifiers) = 0;
};

#endif /* !_UI_SIMULATION_ */