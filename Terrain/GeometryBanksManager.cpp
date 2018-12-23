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

#include "GeometryBanksManager.h"

#include "renderer.h" // for GfxRenderer

namespace Terrain
{
    GeometryBanksManager::GeometryBanksManager(
            const unsigned total_banks_number )
    {
        // Reserve required memory.
        active_geometry_banks.reserve( total_banks_number );
        unreserved_banks     .reserve( total_banks_number );

        // Banks creation.
        for( unsigned int i = 0; i < total_banks_number; ++i )
        {
            active_geometry_banks[ i ] = GfxRenderer.Create_Bank();
            unreserved_banks.push_back( i );
        }
    }

    auto GeometryBanksManager::get_next_geometry_bank() -> gfx::geometry_handle
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

    void GeometryBanksManager::release_bank( gfx::geometry_handle handle )
    { release_bank( handle.bank ); }

    void GeometryBanksManager::release_bank( unsigned int i )
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
};
