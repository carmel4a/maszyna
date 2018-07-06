/*
    This Source Code Form is subject to the
    terms of the Mozilla Public License, v.
    2.0. If a copy of the MPL was not
    distributed with this file, You can
    obtain one at
    http://mozilla.org/MPL/2.0/.
*/

#ifndef _GUI_FSM_
#define _GUI_FSM_

#include "tinyfsm.hpp"

struct PrintLine;

namespace GUI_Events{

    struct Init : public tinyfsm::Event {};
    struct SceneLoaded : public tinyfsm::Event {};
}

struct GUI_FSM : public tinyfsm::Fsm<GUI_FSM>{

    //virtual void react(tinyfsm::Event const &) { }; // generic event
    virtual void react( GUI_Events::Init const & ){};
    virtual void react( PrintLine const & ){};
    virtual void react( GUI_Events::SceneLoaded const & ){};
    
    virtual void entry( void ){};
    virtual void exit( void ){};
};

namespace GUI_States{

    struct Start : GUI_FSM{
        
        void react( GUI_Events::Init const & ) override;
        void entry() override;
        void exit() override;
    };

    struct LoadingScreen : GUI_FSM{

        void react( PrintLine const & e ) override;
        void react( GUI_Events::SceneLoaded const & ) override;
        void entry() override;
        void exit() override;
    };

    struct Simulation : GUI_FSM{

        void entry() override;
        void exit() override;
    };
}

#endif // !_GUI_FSM_