/*
    This Source Code Form is subject to the
    terms of the Mozilla Public License, v.
    2.0. If a copy of the MPL was not
    distributed with this file, You can
    obtain one at
    http://mozilla.org/MPL/2.0/.
*/
#pragma once
#ifndef _FSM_HANDLER_
#define _FSM_HANDLER_
#include "tinyfsm.hpp"
#include "GUI.h"

typedef tinyfsm::FsmList<
    GUI_::GUI_FSM //,
    // other fsm object, ...
> fsm_handler;
struct FSM_{

    template<typename E>
    inline static void send_event(E const & event) {

        fsm_handler::template dispatch<E>(event);
    }
    inline static void start_fsm_list(){
        fsm_handler::start();
    }
};

extern FSM_ FSM ;

#endif /* !_FSM_HANDLER_ */