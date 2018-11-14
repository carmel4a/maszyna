/*
This Source Code Form is subject to the
terms of the Mozilla Public License, v.
2.0. If a copy of the MPL was not
distributed with this file, You can
obtain one at
http://mozilla.org/MPL/2.0/.
*/

#include <memory>
#include <thread>
#include <limits> // for std::numeric_limits
#include <mutex>

#include "Classes.h"
#include "Terrain.h"
#include "renderer.h" // for GfxRenderer
#include "simulation.h" // for simulation::Region
#include "Globals.h" // for camera position
#include "scene.h" // for scene namespace consts

namespace Terrain
{
    Manager::Manager()
    {
        // Init Section list
        // \note IDs of sections start at 0
        for( int id = 0; id < terrain.size(); ++id )
            terrain[ id ].reset( new Terrain::Section( 100, id ) );
        auto& banks = state_lists.active_geometry_banks;
        const unsigned int bank_no = ( update_range + 1 ) * ( update_range + 1 );
        banks.reserve( bank_no );
        state_lists.unreserved_banks.reserve( bank_no );
        for( unsigned int i = 0; i < bank_no; ++i )
        {
            banks[ i ] = GfxRenderer.Create_Bank();
            state_lists.unreserved_banks.push_back( i );
        }
    }

    Manager::~Manager()
    {
        // Send end signal to child threads.
        kill_threads = true;
        // Wait to join child threads.
        for( auto& t : threads ) t.join();
    }

    bool Manager::deserialize( cParser& input )
    {
        threads.emplace_back( &Manager::main_terrain_thread, this );
        return true;
    }

    void Manager::main_terrain_thread()
    {
        constexpr short range = 2 * update_range + 1;

        constexpr int section_side       = scene::SECTION_SIZE;
        constexpr int section_s_num      = scene::REGION_SIDE_SECTION_COUNT;
        constexpr int half_section_s_num = section_s_num / 2;
    
        static int old_camera_start_section_id = -1;
        SectionsContainer temp_active_sections;

        // Thread main loop
        while( !kill_threads )
        {
            // Position relative to origin (0, 0).
            const auto& camera_global_pos = Global.pCamera.Pos;
            /* camera start section is relative to origin. It is not equal
            to `terrain` array, so future origin is needed. So, for center of Region
            we have (254, 254) km */
            const int camera_start_section_x
                    { (int) camera_global_pos.x / section_side };
            const int camera_start_section_y
                    { (int) camera_global_pos.z / section_side };
            const int camera_start_section_id = camera_start_section_x
                    + camera_start_section_y * section_s_num;
            if( camera_start_section_id == old_camera_start_section_id )
                continue;

            old_camera_start_section_id = camera_start_section_id;
            const int x_origin = camera_start_section_x + half_section_s_num;
            const int y_origin = camera_start_section_y + half_section_s_num;

            /* If difference of x OR y coord of camera and section is bigger
            than update range - unload section. */
            for( const auto& id_and_section : m_active_sections )
            {
                const int  id      = id_and_section.first;
                const auto section = id_and_section.second;
                const int  x_coord = id % section_s_num;
                const int  y_coord = id / section_s_num;
                if( std::abs( x_origin - x_coord ) > ( update_range + 1 )
                 || std::abs( y_origin - y_coord ) > ( update_range + 1 ) )
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
                if ( y_origin + y_it  < 0 || y_origin + y_it  > section_s_num ) continue;
                for( int x_it = -1 * range/2; x_it < range; ++x_it )
                {
                    if ( x_origin + x_it  < 0 || x_origin + x_it  > section_s_num ) continue;
                    const int id {
                            ( x_origin + x_it )
                            + ( y_origin + y_it ) * section_s_num };
                    
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

    auto Manager::StateLists::get_next_geometry_bank() -> gfx::geometry_handle
    {
        gfx::geometry_handle return_object;
        if( unreserved_banks.size() > 1 )
        {
            auto return_object = active_geometry_banks[ unreserved_banks.back() ];
            unreserved_banks.pop_back();
            return return_object;
        } else
        {
            active_geometry_banks.push_back( GfxRenderer.Create_Bank() );
            return active_geometry_banks.back();
        }
    }

    void Manager::StateLists::release_bank( gfx::geometry_handle handle )
    { release_bank( handle.bank ); }

    void Manager::StateLists::release_bank( unsigned int i )
    {
        unsigned int it = 0;
        bool fund = false;
        while( it < active_geometry_banks.size() )
        {
            if( active_geometry_banks[it].bank == i )
            {
                fund = true;
                break;
            } else ++it;
        }
        if( !fund ) return;
        GfxRenderer.Release_Bank( active_geometry_banks[it] );
        unreserved_banks.push_back( it );
    }

    Section::Section( int max_side_density, int id )
            : max_side_density { max_side_density }
            , m_id { id }
            , geometry_bank_handle {  }
    {
        const int x = id % scene::REGION_SIDE_SECTION_COUNT;
        const int y = id / scene::REGION_SIDE_SECTION_COUNT;
        m_area.center = { 
                ( (double) ( x - scene::REGION_SIDE_SECTION_COUNT / 2 ) ) * scene::SECTION_SIZE + 0.5 * (double) scene::SECTION_SIZE,
                0.0f,
                ( (double) ( y - scene::REGION_SIDE_SECTION_COUNT / 2 ) ) * scene::SECTION_SIZE + 0.5 * (double) scene::SECTION_SIZE
        };
        m_area.radius = { 707.10678118 }; // radius of the bounding sphere
    }

    bool Section::load( int LOD )
    {

        geometry_bank_handle =
                simulation::Region->terrain()->get_next_geometry_bank();

        m_shapes.emplace_back();
        auto& s = m_shapes.back();
        auto tex = GfxRenderer.Fetch_Material( "ground-yellowbrown" );
        const float size = scene::SECTION_SIZE / (float) max_side_density;
        const int   size_of_tex = 2; // m
        const float samples_per_km = size / (float) size_of_tex;
        s.get_data().rangesquared_min = 0;
        s.get_data().rangesquared_max = 10000'0000;

        s.get_data().lighting.ambient.r = 0.025f;
        s.get_data().lighting.ambient.g = 0.025f;
        s.get_data().lighting.ambient.b = 0.025f;
        s.get_data().lighting.ambient.a = 1.0f;

        s.get_data().lighting.diffuse.r = 0.5f;
        s.get_data().lighting.diffuse.g = 0.5f;
        s.get_data().lighting.diffuse.b = 0.5f;
        s.get_data().lighting.diffuse.a = 1.0f;

        s.get_data().lighting.specular.r = 0.010f;
        s.get_data().lighting.specular.g = 0.010f;
        s.get_data().lighting.specular.b = 0.010f;
        s.get_data().lighting.specular.a = 1.0f;

        s.get_data().material = tex;
        // s.get_data().material = GfxRenderer.Fetch_Material( "grass/grass04" );
        s.get_data().translucent = false;
        
        world_vertex* v = nullptr;

        const float d = 0.5f * scene::SECTION_SIZE;
        for( int chunk_y = 0; chunk_y < max_side_density; ++chunk_y )
            for( int chunk_x = 0; chunk_x < max_side_density; ++chunk_x )
            {
                s.get_data().vertices.emplace_back();
                v = &s.get_data().vertices.back();
                v->position.x = chunk_x * size - d;
                v->position.y = 0.0f;
                v->position.z = chunk_y * size - d;
                v->texture.s  = 0.0f;
                v->texture.t  = 0.0f;

                v->normal.x   = 0.0f;
                v->normal.y   = 1.0f;
                v->normal.z   = 0.0f;
                s.get_data().vertices.emplace_back();
                v = &s.get_data().vertices.back();
                v->position.x = chunk_x * size - d;
                v->position.y = 0.0f;
                v->position.z = (chunk_y + 1) * size - d;
                v->texture.s  = 0.0f;
                v->texture.t  = 1.0f * samples_per_km;

                v->normal.x   = 0.0f;
                v->normal.y   = 1.0f;
                v->normal.z   = 0.0f;
                s.get_data().vertices.emplace_back();
                v = &s.get_data().vertices.back();

                v->position.x = (chunk_x + 1) * size - d;
                v->position.y = 0.0f;
                v->position.z = chunk_y * size - d;
                v->texture.s  = 1.0f * samples_per_km;
                v->texture.t  = 0.0f;

                v->normal.x   = 0.0f;
                v->normal.y   = 1.0f;
                v->normal.z   = 0.0f;
                /////////
                s.get_data().vertices.emplace_back();
                v = &s.get_data().vertices.back();

                v->position.x = (chunk_x + 1) * size - d;
                v->position.y = 0.0f;
                v->position.z = chunk_y * size - d;
                v->texture.s  = 1.0f * samples_per_km;
                v->texture.t  = 0.0f;            

                v->normal.x   = 0.0f;
                v->normal.y   = 1.0f;
                v->normal.z   = 0.0f;
                s.get_data().vertices.emplace_back();
                v = &s.get_data().vertices.back();

                v->position.x = chunk_x * size - d;
                v->position.y = 0.0f;
                v->position.z = (chunk_y + 1) * size - d;
                v->texture.s  = 0.0f;
                v->texture.t  = 1.0f * samples_per_km;

                v->normal.x   = 0.0f;
                v->normal.y   = 1.0f;
                v->normal.z   = 0.0f;
                s.get_data().vertices.emplace_back();
                v = &s.get_data().vertices.back();

                v->position.x = (chunk_x + 1) * size - d;
                v->position.y = 0.0f;
                v->position.z = (chunk_y + 1) * size - d;
                v->texture.s  = 1.0f * samples_per_km;
                v->texture.t  = 1.0f * samples_per_km;

                v->normal.x   = 0.0f;
                v->normal.y   = 1.0f;
                v->normal.z   = 0.0f;
            }
        m_shapes.back().compute_radius();
        m_shapes.back().origin( {0.f, 0.f, 0.f} );
        m_shapes.back().create_geometry( geometry_bank_handle );
        return true;
    }

    bool Section::unload()
    {
        m_shapes.clear();
        simulation::Region->terrain()->release_bank( geometry_bank_handle );
        return true;
    }

    void Section::insert_triangle( scene::shape_node shape )
    {
        shape.compute_radius();
        shape.origin( {0.f, 0.f, 0.f} );

        m_shapes.push_back( shape );
        m_shapes.back().create_geometry( geometry_bank_handle );
    }

    int const Section::side_size_in_meters { scene::SECTION_SIZE };

    Chunk::Chunk( ChunkTypes type )
            : type{ type }
            , center {}
            , vbo {} {}

    EmptyChunk::EmptyChunk()
            : Chunk( ChunkTypes::Empty ) {}

    NormalChunk::NormalChunk()
            : Chunk( ChunkTypes::Normal ) {}
}
