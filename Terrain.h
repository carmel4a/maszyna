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

      private:
        using terrain_array =
                std::array< std::unique_ptr< Section >, scene::SECTIONS_COUNT >;

        terrain_array terrain;
        bool renderer_lock = false;
    };

    class Section
    {
      public:
        Section( int max_side_density );
        int static const side_size_in_meters = 1000;
        int const max_side_density;
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
