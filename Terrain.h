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
#define SCENE_H_19_10_18

#include <vector>

#include "Classes.h"
#include "scene.h" // For `scene` namespace consts.

namespace Terrain
{
    class Manager
    {
      public:
        Manager();
        Manager( Manager& ) = delete;
        Manager& operator=( Manager& ) = delete;
        bool deserialize( cParser& input );
      private:
        using terrain_array =
                std::array< std::unique_ptr< Section >, scene::SECTIONS_COUNT >;

        terrain_array terrain;
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

#endif // !SCENE_H_19_10_18