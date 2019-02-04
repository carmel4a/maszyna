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

#include "./Section.h"

#include "../gl/shader.h"

#include <cstdint> // for CHAR_BIT

#include "./Terrain.h"
#include "../renderer.h" // for GfxRenderer
#include "../Scene/Scene.h" // For scene consts

#ifndef NDEBUG
    #include "gl/error.h"
#endif // !NDEBUG

namespace Terrain
{
    template< typename T >
    struct TypeHelper
    {
        TypeHelper( T t )
                : var { t }
                , size { sizeof(T) } {};
        union Var
        {
            Var( T t) : var { t } {};
            T var;
            unsigned char bytes[ sizeof(T) ];
        } var;
        unsigned size;
    };

    TerrainSectionVertex::TerrainSectionVertex(
            float z,
            glm::vec3 normal,
            char tex )
            : z { z }
            , normal { normal } {};

    void TerrainSectionVertex::store(
            std::vector< unsigned char >& memory ) const
    {
        /// Serialize helper
        /** @param what - attribute to serialize. */
        auto put = []( auto what, std::vector< unsigned char >& where )
        {
            for( unsigned i = 0; i < what.size; ++i )
                where.push_back( what.var.bytes[i] );
        };

        put( TypeHelper<float>( z ), memory );
        put( TypeHelper<float>( normal.x ), memory );
        put( TypeHelper<float>( normal.y ), memory );
        put( TypeHelper<float>( normal.z ), memory );
    };

    Section::Section( unsigned max_side_density, unsigned id )
            : max_side_density { max_side_density }
            , m_id { id }
    {
        const int x = id % scene::REGION_SIDE_SECTION_COUNT;
        const int y = id / scene::REGION_SIDE_SECTION_COUNT;
        m_area.center = { 
                ( (double) ( x - scene::REGION_SIDE_SECTION_COUNT / 2 ) ) * scene::SECTION_SIZE + 0.5 * (double) scene::SECTION_SIZE,
                0.0f,
                ( (double) ( y - scene::REGION_SIDE_SECTION_COUNT / 2 ) ) * scene::SECTION_SIZE + 0.5 * (double) scene::SECTION_SIZE
        };
        m_area.radius = { 5* 707.10678118 + 1 /*hack*/ }; // radius of the bounding sphere
        // vertex_textures.reserve( max_side_density * max_side_density );
    }
    
    bool Section::load()
    {
        textures_to_load.insert( "grass" );
        for( unsigned y = 0; y < max_side_density + 1; ++y )
            for( unsigned x = 0; x < max_side_density + 1; ++x )
                pushVertex( TerrainSectionVertex( 0.1f ));
        return true;
    }

    bool Section::unload()
    {
        return true;
    }

    void Section::moveToVRAM()
    {
        if( !vao ) vao = std::make_unique< gl::vao >();

        vao->bind();
        glCall(glGenBuffers( 1, &vbo ));
        glCall(glBindBuffer( GL_ARRAY_BUFFER, vbo ));

        data.clear();
        data.reserve( size() );
        for( const auto& v : verticies )
            v.store( data );

        unsigned stride = TerrainSectionVertex::size();
        glCall(glBufferData( GL_ARRAY_BUFFER, data.size(), data.data(), GL_STATIC_DRAW ));
        vao->setup_attrib( 0, 1, GL_FLOAT, stride, 0 );
        vao->setup_attrib( 2, 3, GL_FLOAT, stride, sizeof(GLfloat) );

        glCall(glGenBuffers( 1, &ebo ));
        glCall(glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ebo ));

        const unsigned ebo_size = (max_side_density)*(max_side_density)*6;

        auto temp_ebo_data = std::make_unique< std::vector< unsigned > >();
        temp_ebo_data->reserve( ebo_size );
        
        const unsigned vertex_side_count = max_side_density + 1;
        unsigned i = 0;
        for( unsigned y = 0; y < max_side_density; ++y )
            for( unsigned x = 0; x < max_side_density; ++x )
            {
                const unsigned v1 = y*vertex_side_count+x;
                const unsigned v2 = y*vertex_side_count+x+1;
                const unsigned v3 = (y+1)*vertex_side_count+x;
                const unsigned v4 = (y+1)*vertex_side_count+x+1;

                (*temp_ebo_data)[i++] = v1;
                (*temp_ebo_data)[i++] = v2;
                (*temp_ebo_data)[i++] = v3;

                (*temp_ebo_data)[i++] = v2;
                (*temp_ebo_data)[i++] = v4;
                (*temp_ebo_data)[i++] = v3;
            }

        glCall(glBufferData( GL_ELEMENT_ARRAY_BUFFER, ebo_size * sizeof(unsigned), temp_ebo_data->data(), GL_STATIC_DRAW ));

        vao->unbind();
        glCall(glBindBuffer( GL_ARRAY_BUFFER, 0 ));
        glCall(glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 ));
        m_backed = true;
    };

    void Section::deleteFromVRAM()
    {
        glCall(glDeleteBuffers( 1, &vbo ));
        glCall(glDeleteBuffers( 1, &ebo ));
        m_backed = false;
    };

    int const Section::side_size_in_meters { scene::SECTION_SIZE };
}
