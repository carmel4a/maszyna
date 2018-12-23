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
            unsigned id,
            float z,
            glm::vec3 normal,
            char tex )
            : id { id }
            , z { z }
            , normal { normal } {};

    void TerrainSectionVertex::store(
            std::vector< unsigned char >& memory ) const
    {
        /// Serialize helper
        /** @param n - number of bytes.
         *  @param what - attribute to serialize. */

        auto put = []( unsigned n, auto what, std::vector< unsigned char >& where )
        {
            unsigned char temp = '\0';
            for( unsigned byte_number = 0; byte_number < n; ++byte_number )
            {
                temp = '\0';

                for( unsigned i = 0; i < what.size; ++i )
                    temp = what.var.bytes[i];
                where.push_back( temp );
            }
        };

        put( sizeof(unsigned), TypeHelper<unsigned>( id ), memory );
        put( sizeof(float), TypeHelper<float>( z ), memory );
        put( sizeof(float), TypeHelper<float>( normal.x ), memory );
        put( sizeof(float), TypeHelper<float>( normal.y ), memory );
        put( sizeof(float), TypeHelper<float>( normal.z ), memory );
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
        m_area.radius = { 707.10678118 }; // radius of the bounding sphere
        vertex_textures.reserve( max_side_density * max_side_density );
    }
    
    bool Section::load()
    {
        textures_to_load.insert( "grass" );
        for( unsigned y = 0; y < max_side_density; ++y )
            for( unsigned x = 0; x < max_side_density; ++x )
                pushVertex( TerrainSectionVertex( y * max_side_density + x, 0.1f ));
                vertex_textures.push_back( getTexture( "grass" ) );
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
        glGenBuffers( 1, &vbo );
        glBindBuffer( GL_ARRAY_BUFFER, vbo );
        std::cout << "VBO: " << vbo;
        std::cout.flush();

        glGenBuffers( 1, &ebo );
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ebo );
        std::cout << "EBO: " << ebo;
        std::cout.flush();
        data.clear();
        data.reserve( size() );
        for( const auto& v : verticies )
            v.store( data );

        glBufferData( GL_ARRAY_BUFFER, size(), data.data(), GL_STATIC_DRAW );
        unsigned stride = sizeof(GL_UNSIGNED_INT)
                          + 4* sizeof(GL_FLOAT)
                          + sizeof(GL_UNSIGNED_BYTE);
        vao->setup_attrib( 0, 1, GL_UNSIGNED_INT, stride, 0 );
        vao->setup_attrib( 1, 1, GL_FLOAT, stride, sizeof(GL_UNSIGNED_INT) );
        vao->setup_attrib( 2, 3, GL_FLOAT, stride, sizeof(GL_UNSIGNED_INT) + sizeof(GL_FLOAT) );

        unsigned temp_ebo_data[ (max_side_density)*(max_side_density)*6 ];
        short i = 0;
        for( unsigned y = 0; y < max_side_density; ++y )
            for( unsigned x = 0; x < max_side_density; ++x )
            {
                const unsigned v1 = y*max_side_density+x;
                const unsigned v2 = y*max_side_density+x+1;
                const unsigned v3 = (y+1)*max_side_density+x;
                const unsigned v4 = (y+1)*max_side_density+x+1;

                temp_ebo_data[i++] = v1;
                temp_ebo_data[i++] = v2;
                temp_ebo_data[i++] = v3;

                temp_ebo_data[i++] = v2;
                temp_ebo_data[i++] = v4;
                temp_ebo_data[i++] = v1;
            }

        glBufferData( GL_ELEMENT_ARRAY_BUFFER, size(), &temp_ebo_data, GL_STATIC_DRAW );

        vao->unbind();
        glBindBuffer( GL_ARRAY_BUFFER, 0 );
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
        m_backed = true;
    };

    void Section::deleteFromVRAM()
    {
        glDeleteBuffers( 1, &vbo );
        m_backed = false;
    };

    int const Section::side_size_in_meters { scene::SECTION_SIZE };
}
