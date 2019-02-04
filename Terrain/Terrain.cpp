/*
This Source Code Form is subject to the
terms of the Mozilla Public License, v.
2.0. If a copy of the MPL was not
distributed with this file, You can
obtain one at
http://mozilla.org/MPL/2.0/.
*/
/** \file
*/

#include "./Section.h"
#include "./Terrain.h"
#include "./Config.hpp"
#include "../stdafx.h"
#include "./Threads/Tasks/Batching.hpp"

using Terrain::Manager;
using Terrain::BatchingTask;

Manager::Manager()
        : mutexes {  }
        , terrain { max_side_density }
        , m_active_sections {  }
        , threads { 1 }
        , camera_section_id {  }
{
    // @todo OpenGL context init
}

Manager::~Manager()
{
    // Send end signal to child threads.
    threads.kill = true;
    // Wait to join child threads.
    for( auto& t : threads.workers ) t.join();

    // @todo OpenGL context delete
}

bool Manager::deserialize( cParser& input )
{
    // rest of deserialization. load from file, heightmap?
    threads.pushTask< BatchingTask >( this );
    return true;
}

bool Manager::serialize( cParser& output )
{
    return false;
}
