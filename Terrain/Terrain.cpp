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
#include "../simulation.h" // for simulation::Region
#include "../Globals.h" // for camera position
#include "../Scene/SceneConfig.h" // for scene namespace consts
#include "../Classes.h"
#include "../stdafx.h"

#include "glm/glm.hpp"

using Terrain::Manager;




const unsigned Manager::max_side_density { 5 };
const unsigned Manager::update_range { 3 };
const unsigned Manager::banks_number { update_range * update_range };
const unsigned Manager::additional_banks { 5 };

Manager::Manager()
        : mutexes {  }
        , terrain { max_side_density }
        , m_geometry_bank_manager { banks_number + additional_banks }
        , m_active_sections {  }
        , threads {  }
        , kill_threads { false } {}

Manager::~Manager()
{
    // Send end signal to child threads.
    kill_threads = true;
    // Wait to join child threads.
    for( auto& t : threads ) t.join();
}

bool Manager::deserialize( cParser& input )
{
    // rest of deserialization. load from file, heightmap?
    threads.emplace_back( &Manager::main_terrain_thread, this );
    return true;
}

void Manager::main_terrain_thread()
{
    // Update square side.
    /**
     *  ...........
     *  .."""|""".. ^ ^
     *  .."""|""".. | |  update_range = 3
     *  .."""|""".. | V
     *  ..---X---.. |    range = 7
     *  .."""|""".. |
     *  .."""|""".. |
     *  .."""|""".. V
     *  ...........
     */
    constexpr short range = 2 * update_range + 1;

    // Section side size (in meters).
    constexpr int section_side       = scene::SECTION_SIZE;
    // Region sections side number.
    constexpr int section_s_num      = scene::REGION_SIDE_SECTION_COUNT;
    // Half of Region sections side number.
    constexpr int half_section_s_num = section_s_num / 2;

    static unsigned int old_camera_start_section_id =
            std::numeric_limits< unsigned >::max();

    SectionsContainer temp_active_sections;

    // Thread main loop
    while( !kill_threads )
    {
        // (Global) Position relative to origin (0, 0).
        const auto& camera_pos = Global.pCamera.Pos;

        auto start_section = [&section_side]( int a ) -> int
        { return (int) a / section_side; };

        // Camera Starting Section
        glm::ivec2 s_section( start_section( camera_pos.x ),
                             start_section( camera_pos.z ) );

        const int s_section_id = s_section.x + s_section.y * section_s_num;

        // If user didn't moved to other section, there's nothing to do.
        if( s_section_id == old_camera_start_section_id ) continue;

        old_camera_start_section_id = s_section_id;

        /* camera start section is relative to origin. It is not equal
        to `terrain` array, so this move of origin is needed. So, for center
        of Region we have (250, 250) km */
        const glm::ivec2 origin =
                s_section+ glm::ivec2( half_section_s_num, half_section_s_num);

        /* If difference of x OR y coord of camera and section is bigger
        than update range - unload section. */
        for( const auto& id_and_section : m_active_sections )
        {
            const int  id      = id_and_section.first;
            const auto section = id_and_section.second;
            const glm::ivec2 coord( id % section_s_num, id / section_s_num );
            const glm::ivec2 delta = origin - coord;

            if( std::abs( delta.x ) > update_range
                || std::abs( delta.y ) > update_range )
            {
                std::scoped_lock< std::mutex > lock ( mutexes.section_unload );

                section->unload();
                if( temp_active_sections.find( id )
                    != temp_active_sections.end() )
                temp_active_sections.erase( id );
            }
        }

        for( int y_it = -1 * range/2; y_it < range; ++y_it )
        {
            if ( origin.y + y_it  < 0 || origin.y + y_it  > section_s_num ) continue;
            for( int x_it = -1 * range/2; x_it < range; ++x_it )
            {
                if ( origin.x + x_it  < 0 || origin.x + x_it  > section_s_num ) continue;
                const int id {
                        ( origin.x + x_it )
                        + ( origin.y + y_it ) * section_s_num };
                
                /* \todo check if renderer doesn't actually rendere old
                sections. if it do, new section may be added inside of vbo,
                not at the end and renderer may thread new data as old
                shape. or something like that */
                terrain[ id ]->load();
                temp_active_sections[ id ] = terrain[ id ].get();
            }
        }

        {
            std::scoped_lock< std::mutex > lock ( mutexes.active_list_swap );
            m_active_sections.swap( temp_active_sections );
        }
        temp_active_sections.clear();
    }
}
