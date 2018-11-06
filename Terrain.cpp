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

    Section::Section( int max_side_density )
            : max_side_density { max_side_density }
    {
        chunks.reserve( max_side_density * max_side_density );
        for( int i = 0; i < max_side_density * max_side_density; ++i )
            chunks.emplace_back( new EmptyChunk() );
    }
    
    Chunk::Chunk( ChunkTypes type )
            : type{ type }
            , center {}
            , vbo {} {}

    EmptyChunk::EmptyChunk()
            : Chunk( ChunkTypes::Empty ) {}

    NormalChunk::NormalChunk()
            : Chunk( ChunkTypes::Normal ) {}
}
