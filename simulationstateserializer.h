/*
This Source Code Form is subject to the
terms of the Mozilla Public License, v.
2.0. If a copy of the MPL was not
distributed with this file, You can
obtain one at
http://mozilla.org/MPL/2.0/.
*/

#pragma once

#include "parser.h"
#include "Scene/Scene.h"

namespace simulation {

class state_serializer {

public:
// methods
    // restores simulation data from specified file. returns: true on success, false otherwise
    bool
        deserialize( std::string const &Scenariofile );
    // stores class data in specified file, in legacy (text) format
    void
        export_as_text( std::string const &Scenariofile ) const;

private:
// methods
    // restores class data from provided stream
    void deserialize( cParser &Input, Scene::scratch_data &Scratchpad );
    void deserialize_area( cParser &Input, Scene::scratch_data &Scratchpad );
    void deserialize_atmo( cParser &Input, Scene::scratch_data &Scratchpad );
    void deserialize_camera( cParser &Input, Scene::scratch_data &Scratchpad );
    void deserialize_config( cParser &Input, Scene::scratch_data &Scratchpad );
    void deserialize_description( cParser &Input, Scene::scratch_data &Scratchpad );
    void deserialize_event( cParser &Input, Scene::scratch_data &Scratchpad );
    void deserialize_lua( cParser &Input, Scene::scratch_data &Scratchpad );
    void deserialize_firstinit( cParser &Input, Scene::scratch_data &Scratchpad );
    void deserialize_group( cParser &Input, Scene::scratch_data &Scratchpad );
    void deserialize_endgroup( cParser &Input, Scene::scratch_data &Scratchpad );
    void deserialize_light( cParser &Input, Scene::scratch_data &Scratchpad );
    void deserialize_node( cParser &Input, Scene::scratch_data &Scratchpad );
    void deserialize_origin( cParser &Input, Scene::scratch_data &Scratchpad );
    void deserialize_endorigin( cParser &Input, Scene::scratch_data &Scratchpad );
    void deserialize_rotate( cParser &Input, Scene::scratch_data &Scratchpad );
    void deserialize_sky( cParser &Input, Scene::scratch_data &Scratchpad );
    void deserialize_test( cParser &Input, Scene::scratch_data &Scratchpad );
    void deserialize_time( cParser &Input, Scene::scratch_data &Scratchpad );
    void deserialize_trainset( cParser &Input, Scene::scratch_data &Scratchpad );
    void deserialize_endtrainset( cParser &Input, Scene::scratch_data &Scratchpad );
    TTrack * deserialize_path( cParser &Input, Scene::scratch_data &Scratchpad, Scene::node_data const &Nodedata );
    TTraction * deserialize_traction( cParser &Input, Scene::scratch_data &Scratchpad, Scene::node_data const &Nodedata );
    TTractionPowerSource * deserialize_tractionpowersource( cParser &Input, Scene::scratch_data &Scratchpad, Scene::node_data const &Nodedata );
    TMemCell * deserialize_memorycell( cParser &Input, Scene::scratch_data &Scratchpad, Scene::node_data const &Nodedata );
    TEventLauncher * deserialize_eventlauncher( cParser &Input, Scene::scratch_data &Scratchpad, Scene::node_data const &Nodedata );
    TAnimModel * deserialize_model( cParser &Input, Scene::scratch_data &Scratchpad, Scene::node_data const &Nodedata );
    TDynamicObject * deserialize_dynamic( cParser &Input, Scene::scratch_data &Scratchpad, Scene::node_data const &Nodedata );
    sound_source * deserialize_sound( cParser &Input, Scene::scratch_data &Scratchpad, Scene::node_data const &Nodedata );
    // skips content of stream until specified token
    void skip_until( cParser &Input, std::string const &Token );
    // transforms provided location by specifed rotation and offset
    glm::dvec3 transform( glm::dvec3 Location, Scene::scratch_data const &Scratchpad );
};

} // simulation

//---------------------------------------------------------------------------
