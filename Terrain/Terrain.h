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

#ifndef TERRAIN_H_19_10_18
#define TERRAIN_H_19_10_18

#pragma once

#include <vector>
#include <unordered_map>
#include <thread>
#include <mutex>

#include "Classes.h"
#include "scene.h" // For `scene` namespace consts.
#include "Terrain/Chunk.h" // Currently to delete.

/// Namespace for terrain entities.
namespace Terrain
{
    using SectionsContainer = std::unordered_map<
            unsigned int,
            Terrain::Section* >;
    /// Entry point to Terrain management.
    /** \note no copyable. */
    class Manager
    {
        // TBD: move get/reset geometry buffer to private
        // friend Section::load;
        // friend Section::unload;
      public:
      // Special member functions
        Manager();
        ~Manager();
        Manager( Manager& ) = delete;
        Manager& operator=( Manager& ) = delete;

      // Types
        class TerrainContainerResource
        {
            friend Manager;
          public:
            inline auto list() -> SectionsContainer& { return m_list; }
            inline auto list() const -> const SectionsContainer& { return m_list; }
            operator std::mutex&() { return mutex; }
          private:
            SectionsContainer m_list;
            std::mutex mutex;
        };

        class GeometryBanksManager
        {
            friend Manager;
            auto get_next_geometry_bank() -> gfx::geometry_handle;
            void release_bank( gfx::geometry_handle handle );
            void release_bank( unsigned int i );

            std::vector< gfx::geometry_handle > active_geometry_banks;
            std::vector< unsigned int > unreserved_banks;
        };

        struct Mutexes
        {
            std::mutex active_list_swap;
            std::mutex section_unload;
        };

      // Serialization
        /// Deserialize terrain from provided parser.
        bool deserialize( cParser& input ); // TO IMPLEMENT

        /// Serialize terrain to provided parser.
        /** Possbile side efects in filesystem. */
        bool serialize( cParser& input ) { return false; }; // TO IMPLEMENT

      // Geometry bank management
        inline auto get_next_geometry_bank() -> gfx::geometry_handle
        { return geometry_banks.get_next_geometry_bank(); };
        inline void release_bank( gfx::geometry_handle handle )
        { geometry_banks.release_bank( handle.bank ); };
        inline void release_bank( unsigned int i )
        { geometry_banks.release_bank( i ); };

      // Section access
        inline auto active_sections() const -> const SectionsContainer&
        { return m_active_sections; };

      // Data
        GeometryBanksManager geometry_banks;
        Mutexes mutexes;

      private:
      // Types
        using terrain_array =
                std::array< std::unique_ptr< Section >, scene::SECTIONS_COUNT >;

      // Data
        /// Half side of update sections square.
        constexpr static short update_range { 3 };

        SectionsContainer m_active_sections;
        terrain_array terrain;

      // Threads
        void main_terrain_thread();
        std::vector< std::thread > threads;
        bool kill_threads = false;
    };

    class Section
    {
      public:
      // Special member functions
        Section( int max_side_density, int id );

      // Getters
        inline auto id() const -> const int { return m_id; }
        inline auto area() const -> const scene::bounding_area& { return m_area; }
        inline auto shapes() const -> const std::vector< scene::shape_node >& { return m_shapes; }

      // Serialization
        bool load( int LOD = 0 );
        bool unload();

      // Data management
        void insert_triangle( scene::shape_node );

      private:
      // Data
        int m_id;
        gfx::geometrybank_handle                geometry_bank_handle;
        scene::bounding_area                    m_area;
        std::vector< scene::shape_node >        m_shapes;
        static const int                        side_size_in_meters;
        const int                               max_side_density;
        std::vector< std::unique_ptr< Chunk > > chunks;
    };
}

#endif // !TERRAIN_H_19_10_18
