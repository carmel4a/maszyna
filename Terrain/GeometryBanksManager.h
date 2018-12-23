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

// Java style ;d
#ifndef TERRAIN_GEOMETRY_BANKS_MANAGER_H_19_10_18
#define TERRAIN_GEOMETRY_BANKS_MANAGER_H_19_10_18
#pragma once

#include "../stdafx.h"
#include "../renderer.h" // For gfx::geometry_handle

namespace Terrain
{
    class GeometryBanksManager
    {
      public:
        ///
        GeometryBanksManager( const unsigned total_banks_number );

        /// Gets free geometry bank.
        /** Gets handle to next free geometry bank.
         *  @return next free geometry handle. If no was available,
         *  @ref null_handle is returned. */
        auto get_next_geometry_bank() -> gfx::geometry_handle;

        /// Releases provided geometry bank.
        /** Releases bank's memory, and sets it again as available for usage.
         *  @param handle - handle which we want to release. */
        void release_bank( gfx::geometry_handle handle );
        
        /// Releases provided geometry bank.
        /** Releases bank's memory, and sets it again as available for usage.
         *  @param handle - numerical value of handle to release. */
        void release_bank( unsigned int i );

      private:
        std::vector< gfx::geometry_handle > active_geometry_banks;
        std::vector< unsigned int > unreserved_banks;
    };
};

#endif // !TERRAIN_GEOMETRY_BANKS_MANAGER_H_19_10_18
