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

#ifndef TERRAIN_CONTAINER_H_23_12_18
#define TERRAIN_CONTAINER_H_23_12_18
#pragma once

#include "../stdafx.h"
#include "../Classes.h"
#include "../Scene/SceneConfig.h"

namespace Terrain
{
    /// Section s array wrapper.
    /** Main goal is to proide Section s auto construction.
     *  @note no copyable, as copying full Sections array currently has no 
     *  sense. */
    class TerrainContainer
    {
      public:
      ///@{ @name Special member functions
        /// Constructor
        /** Creates [pre-defined number](@ref scene::SECTIONS_COUNT) of
         *  Section s, and store them in [static array](@ref content).
         *
         *  @param max_side_density - maximum number how much times an section
         *  may be divided in subdivide process ie. quad tree, or tileing. */
        TerrainContainer( const unsigned max_side_density );

        /// Default destructor.
        ~TerrainContainer() = default; ///@}

      ///@{ @name Types
        /// Internal data structure.
        using Sequence =
                std::array< std::unique_ptr< Section >, scene::SECTIONS_COUNT >;

      /**@}*///**@ @name Getters *//
        /// Getter to intern data.
        /** @param i - index, starting from `0` to @ref scene::SECTIONS_COUNT.
         *  @note no special range checks are done. */
        inline auto operator[]( unsigned i ) -> std::unique_ptr< Section >&;
        ///@}

      private:
        Sequence content; ///< Main data.
    };

    inline auto TerrainContainer::operator[]( unsigned i )
    -> std::unique_ptr< Section >&
    { return content[i]; };
}

#endif // !TERRAIN_CONTAINER_H_23_12_18
