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

/** @defgroup Terrain Terrain
 * 
 *  Terrain (ground level, water, buildings, static objects) is divided on
 *  Section s.
 *  
 *  Section consist of array of quads (2 triangles). Manager::max_side_density
 *  determines maximum number of quads. Terrain::Section should replace
 *  scene::Section. */

#ifndef TERRAIN_H_19_10_18
#define TERRAIN_H_19_10_18

#pragma once

#include "./TerrainContainer.h"
#include "./GeometryBanksManager.h"
#include "./Types.h"
#include "../stdafx.h"
#include "../Scene/SceneConfig.h"

/// Namespace for terrain entities.
/** @addtogroup Terrain */
namespace Terrain
{
    class TerrainTask
    {
      public:
        TerrainTask() = default;
        void run();
    };

    /// Entry point to Terrain management.
    /** @note no copyable.
     *  @todo move get/reset geometry buffer to private, like:
     *  ```C++
     *      friend Section::load;
     *      friend Section::unload;
     *  ```
     *  @todo Move thread creation to (no existing) global thread manager.
     *  @addtogroup Terrain */
    class Manager
    {
      public:
      ///@{ @name Special member functions
        /// Default constructor.
        /** Fills @ref terrain array with newly created Secton s.
         *  
         *  Creates required [geometry banks](@ref geometry_banks). */
        Manager();

        /// Destructor
        /** Joins created threads. */
        ~Manager();

        /// Prevention of copying.
        Manager( Manager& ) = delete;

        /// Prevention of copying.
        Manager& operator=( Manager& ) = delete; ///@}

      ///@{ @name Types
        /// Struct to store mutexes.
        struct Mutexes
        {
            /// Guard swap @ref m_active_sections.
            /** Is locked on swap shared [sections list](@ref m_active_sections)
            with [renderer](@ref opengl_renderer). */
            std::mutex active_list_swap,
                       section_unload;
        }; ///@}

      ///@{ @name Serialization
        /// Deserialize terrain from provided parser.
        /** @param input - input from where Manager should read data.
         *  @return if deserialization was successfull.
         *  @todo To implement. */
        bool deserialize( cParser& input );

        /// Serialize terrain to provided parser.
        /** Possbile side efects in filesystem.
         *  @param output - where data will be stored. May be stdout and a file.
         *  @return if serialization was successfull.
         *  @todo To implement. */
        bool serialize( cParser& output ) { return false; }; ///@}

      ///@{ @name Getters
        ///
        inline auto geometry_bank_manager() -> GeometryBanksManager&;

        /// Gets @ref m_active_sections.
        /** @return container of pairs witch id, and pointer, to `active
         *  sections`. */
        inline auto active_sections() const -> const SectionsContainer&; ///@}

      ///@{ @name Data
        /// Public mutexes.
        /** Currenlty used by renderer. */
        Mutexes mutexes; ///@}

      private:
      ///@{ @name Consts
        /// Max quads number on side of a Section.
        static const unsigned max_side_density;
        /// Half side of update sections square.
        static const unsigned update_range;

        /// Max banks number used by terrain manager.
        static const unsigned banks_number;
        /// Number of additional banks used when banks_number isn't sufficient.
        static const unsigned additional_banks; ///@}

      ///@{ @name Data
        /// Geometry banks used by Section s.
        GeometryBanksManager m_geometry_bank_manager;
        ///
        TerrainContainer terrain;
        SectionsContainer m_active_sections; ///@}

      /// @{ Threads
        void main_terrain_thread();
        std::vector< std::thread > threads;
        bool kill_threads; ///@}
    };

    inline auto Manager::geometry_bank_manager() -> GeometryBanksManager&
    { return m_geometry_bank_manager; }

    inline auto Manager::active_sections() const -> const SectionsContainer&
    { return m_active_sections; }
}

#endif // !TERRAIN_H_19_10_18
