/*
This Source Code Form is subject to the
terms of the Mozilla Public License, v.
2.0. If a copy of the MPL was not
distributed with this file, You can
obtain one at
http://mozilla.org/MPL/2.0/.
*/

#pragma once

#ifndef TERRAIN_H_19_10_18
#define TERRAIN_H_19_10_18

#include <vector>
#include <unordered_map>
#include <thread>
#include <mutex>

#include "Classes.h"
#include "scene.h" // For `scene` namespace consts.

/// Namespace for terrain entities.
namespace Terrain
{
    /// Entry point to Terrain management.
    /** \note no copyable. */
    class Manager
    {
        // TBD: move get/reset geometry buffer to private
        // friend Section::load;
        // friend Section::unload;
      public:
        Manager();
        ~Manager();
        Manager( Manager& ) = delete;
        Manager& operator=( Manager& ) = delete;
        using terrain_vector = std::unordered_map< unsigned int, Section* >;

        /// Deserialize terrain from provided parser.
        bool deserialize( cParser& input ); // TO IMPLEMENT

        class TerrainVector
        {
            friend Manager;
          public:
            inline auto list() -> terrain_vector& { return m_list; }
            inline auto list() const -> const terrain_vector& { return m_list; }
            inline void lock()
            { while( ! mutex.try_lock() ) continue; };
            inline void unlock()
            { mutex.unlock(); };
          private:
            terrain_vector m_list;
            std::mutex mutex;
        };

        inline void render_lock()
        { renderer_lock = true; };
        inline void render_unlock()
        { renderer_lock = false; };
        inline bool is_render_locked()
        { return renderer_lock; }

        inline auto active() const -> const TerrainVector&
        { return state_lists.m_active; };
        inline auto active() -> TerrainVector&
        { return state_lists.m_active; };
        inline auto to_load() const -> const TerrainVector&
        { return state_lists.m_to_load; };
        inline auto to_load() -> TerrainVector&
        { return state_lists.m_to_load; };
        inline auto to_unload() const -> const TerrainVector&
        { return state_lists.m_to_unload; };
        inline auto to_unload() -> TerrainVector&
        { return state_lists.m_to_unload; };

        // Geometry bank management
        inline auto get_next_geometry_bank() -> gfx::geometry_handle
        { return state_lists.get_next_geometry_bank(); };
        inline void release_bank( gfx::geometry_handle handle )
        { state_lists.release_bank( handle.bank ); };
        inline void release_bank( unsigned int i )
        { state_lists.release_bank( i ); };

        class StateLists
        {
            friend Manager;
            auto get_next_geometry_bank() -> gfx::geometry_handle;
            void release_bank( gfx::geometry_handle handle );
            void release_bank( unsigned int i );

            TerrainVector m_to_unload;
            TerrainVector m_active;
            TerrainVector m_to_load;

            std::vector< gfx::geometry_handle > active_geometry_banks;
            std::vector< unsigned int > unreserved_banks;
        } state_lists;
      private:
        using terrain_array =
                std::array< std::unique_ptr< Section >, scene::SECTIONS_COUNT >;

        // Threads
        void main_terrain_thread();

        /// Half side of update sections square.
        constexpr static short update_range { 3 };

        terrain_array terrain;

        std::vector< std::thread > threads;
        bool kill_threads = false;
        bool renderer_lock = false;
    };

    class Section
    {
      public:
        Section( int max_side_density );
        int static const side_size_in_meters = 1000;
        int const max_side_density;

      private:
        gfx::geometrybank_handle geometry_bank_handle;
        std::vector< std::unique_ptr< Chunk > > chunks;
    };
    
    enum class ChunkTypes : short
    {
        Abstract,
        Normal,
        Empty
    };

    class Chunk
    {
      public:
        Chunk( ChunkTypes );
      protected:
        const ChunkTypes type;
        const glm::vec3 center;
        gfx::geometrybank_handle vbo;
    };

    class EmptyChunk : public Chunk
    {
      public:
        EmptyChunk();
    };

    class NormalChunk : public Chunk
    {
      public:
        NormalChunk();
    };
}

#endif // !TERRAIN_H_19_10_18
