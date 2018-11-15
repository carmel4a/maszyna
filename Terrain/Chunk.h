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

#ifndef SECTION_CHUNK_14_11_18
#define SECTION_CHUNK_14_11_18

#pragma once

#include "openglgeometrybank.h" // for geometrybank_handle

namespace Terrain
{
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
        const ChunkTypes         type;
        const glm::vec3          center;
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

#endif // !SECTION_CHUNK_14_11_18
