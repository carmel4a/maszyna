/*
This Source Code Form is subject to the
terms of the Mozilla Public License, v.
2.0. If a copy of the MPL was not
distributed with this file, You can
obtain one at
http://mozilla.org/MPL/2.0/.
*/
/** @file
 * 
 */

#include "./Batching.hpp"

#include "../../Terrain.h"
#include "../../Section.h"

using Terrain::BatchingTask;
using glm::ivec2;

BatchingTask::BatchingTask( Manager* const manager )
        : manager { manager }
        , sections {  } {}

void BatchingTask::run()
{
    // (Global) Position relative to (world space) origin (0, 0).
    const auto& camera_pos = Global.pCamera.Pos;
    
    // Camera Starting Section
    const ivec2 camera_section
            { Manager::getSectionCoordFromPos(
                    ivec2( Global.pCamera.Pos.x, Global.pCamera.Pos.z ) ) };

    const unsigned camera_section_id
            { Manager::getSectionIDFromCoord( camera_section ) };

    // If user didn't moved to other section, there's nothing to do.
    if( camera_section_id == manager->getCameraSectionId() ) return;
    manager->setCameraSectionId( camera_section_id );

    unloadSections( camera_section );
    loadSections( camera_section );
    swapSectionsWithManager();
}

constexpr bool BatchingTask::isInfinite() const { return true; };

auto BatchingTask::copy() const -> TaskPtr
{ return std::make_unique< BatchingTask >( manager ); }

void BatchingTask::loadSections( glm::ivec2 origin )
{
    for( int y = -range/2; y <= range/2; ++y )
    {
        if( origin.y + y < 0 || origin.y + y > section_s_num ) continue;
        for( int x = -range/2; x <= range/2; ++x )
        {
            if( origin.x + x < 0 || origin.x + x > section_s_num ) continue;

            sections.to_load.insert(
                    ( origin.x + x ) + ( origin.y + y ) * section_s_num );
        }
    }
}

void BatchingTask::unloadSections( ivec2 origin )
{
    /* If difference of x OR y coord of camera and section is bigger
    than update range - unload section. */
    for( const auto& id_and_section : manager->m_active_sections )
    {
        const auto [id, section] = id_and_section;
        const ivec2 coord( id % section_s_num, id / section_s_num );
        const ivec2 delta = origin - coord;

        if( std::abs( delta.x ) > manager->update_range
         || std::abs( delta.y ) > manager->update_range )
        {
            sections.to_unload.insert( id );
        }
    }
}

void BatchingTask::swapSectionsWithManager()
{
    if( !sections.to_unload.empty() )
    {
        std::scoped_lock< std::mutex > lock { manager->mutexes.section_unload };
        for( auto s : sections.to_unload )
        {
            if( sections.to_load.find( s ) != sections.to_load.end() ) continue;
            manager->terrain[ s ]->unload();
            auto it = sections.active.find( s );
            if( it != sections.active.end() )
                sections.active.erase( it );
        }
    }

    if( !sections.to_load.empty() )
        for( auto s : sections.to_load )
        {
            // if( sections_to_unload.find( s ) != sections_to_unload.end() ) continue;
            auto& section = manager->terrain[ s ];
            section->load();
            sections.active[ s ] = section.get();
        }

    sections.to_load.clear();
    sections.to_unload.clear();

    {
        std::scoped_lock< std::mutex, std::mutex > lock
                ( manager->mutexes.active_list_swap,
                  manager->mutexes.section_unload );
        manager->m_active_sections.swap( sections.active );
    }
}
