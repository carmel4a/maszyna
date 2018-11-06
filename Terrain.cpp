/*
This Source Code Form is subject to the
terms of the Mozilla Public License, v.
2.0. If a copy of the MPL was not
distributed with this file, You can
obtain one at
http://mozilla.org/MPL/2.0/.
*/

#include <memory>

#include "Classes.h"
#include "Terrain.h"
#include "renderer.h" // for GfxRenderer
#include "simulation.h" // for simulation::Region
namespace Terrain
{
    Manager::Manager()
    {
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
        // Thread main loop
        while( !kill_threads )
        {
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
        while( true )
        {
            if ( ! simulation::Region->terrain()->is_render_locked() )
            {
                geometry_bank_handle =
                        simulation::Region->terrain()->get_next_geometry_bank();
                break;
            }
            else continue;
        }

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
        while( true )
        {
            if ( ! simulation::Region->terrain()->is_render_locked() ) { m_shapes.clear();; break; }
            else continue;
        }
        while( true )
        {
            if ( ! simulation::Region->terrain()->is_render_locked() ) { simulation::Region->terrain()->release_bank( geometry_bank_handle );; break; }
            else continue;
        }
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
