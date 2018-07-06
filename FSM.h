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
#include "GUI_FSM.h"

/// FSMs list in project.
/** If you created new FSM, add it here. Don't touch it in other reasons if it
 *  is not necessary. 
 */
typedef tinyfsm::FsmList<
    GUI_FSM
    // other fsm object, ...
> fsm_handler;

/// FSM entry class.
/** Actually used only for sending signals. Usage: `FSM::send_event( NameOfEvent( opt_args ) );` */
struct FSM{

    template<typename E>
    inline static void send_event(E const & event) {

        fsm_handler::template dispatch<E>(event);
    }
    /** Don't touch if it is not necessary. Should be private, but must be used
     *  in `EU07.cpp` . 
     */
    inline static void start_fsm_list(){
        fsm_handler::start();
    }
};

#endif /* !_FSM_HANDLER_ */