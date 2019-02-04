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

#ifndef TERRAIN_SECTION_15_11_18
#define TERRAIN_SECTION_15_11_18

#pragma once

#include "../stdafx.h"
#include "../scenenode.h" // For bounding_area, shape_node
#include "../openglgeometrybank.h" // for geometrybank_handle
#include "../simulation.h" // For simulation::Region
#include "../Classes.h"
#include "../renderer.h"

#include <../ref/glfw/include/GLFW//glfw3.h>
#include "glm/glm.hpp"

#include "../gl/vao.h"

namespace Terrain
{
    struct TerrainSectionVertex
    {
        TerrainSectionVertex() = delete;
        TerrainSectionVertex(
                float z,
                glm::vec3 normal=glm::vec3(0, 1, 0),
                char tex=0b0 );

        static inline constexpr auto size() -> unsigned;
        void store( std::vector< unsigned char >& memory ) const;

      private:
        unsigned id;
        float z;
        glm::vec3 normal;
    };
    struct SectionShader
    {
        SectionShader() : id { 0 }
        {
            std::string vert = gl::shader::read_file( "terrain.vert" );
            std::string frag = gl::shader::read_file( "terrain.frag" );
            GLuint vs = glCreateShader( GL_VERTEX_SHADER );
            GLuint fs = glCreateShader( GL_FRAGMENT_SHADER );
            const char* vs_s = vert.data();
            glShaderSource(vs, 1, &vs_s, NULL);
            glCompileShader(vs);
            int  success;
            char infoLog[512];
            glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
            if(!success)
            {
                glGetShaderInfoLog(vs, 512, NULL, infoLog);
                std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
                throw;
            }
            const char* fs_s = frag.data();
            glShaderSource(fs, 1, &fs_s, NULL);
            glCompileShader(fs);
            glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
            if(!success)
            {
                glGetShaderInfoLog(fs, 512, NULL, infoLog);
                std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
                throw;
            }


            id = glCreateProgram();

            glAttachShader(id, vs);
            glAttachShader(id, fs);
            glLinkProgram(id);
        };

        operator GLuint()
        {
            return id;
        };
        GLuint id;
    };

    static GLuint getSectionShader()
    {   
        static SectionShader s;
        return s;
    };

    ///
    /** */
    class Section
    {
      public:
      ///@{ @name Special member functions
        Section( unsigned max_side_density, unsigned id );
        Section( Section& ) = delete;
        Section& operator=( Section& ) = delete; ///@}

        inline bool operator==( const Section& s );

        struct Mutexes
        {
            std::mutex data_swap;
        } mutexes;

      ///@{ Getters
        inline auto id() const -> int;
        inline auto area() const -> const scene::bounding_area&; ///@}

      ///@{ @name Memory management.
        bool load();
        bool unload(); ///@}

      ///@{ @name Serialization
        void serialize( std::ostream ) const;
        void deserialize( std::istream ); ///@}

      // Render management
        void moveToVRAM();
        void deleteFromVRAM();

        bool backed() { return m_backed; }
        auto getTexture( std::string path ) const -> GLuint
        { return textures.at( path ); };

        void loadTexture( std::string path )
        {
            textures[ path ] = GfxRenderer.Fetch_Texture( path );
        };

        void freeTextures()
        {

        };

        auto getSideSize() const -> unsigned { return side_size_in_meters; };
        auto getVAO() const -> GLuint { return *vao; };
        auto getEBO() const -> GLuint { return ebo; };
        auto elementsCount() const -> unsigned { return max_side_density*max_side_density*6; };
        auto getShader() const -> GLuint { return getSectionShader(); };

        void setData( std::vector< unsigned char >&& new_data )
        {
            std::scoped_lock< std::mutex > lock( mutexes.data_swap );
            data.swap( new_data );
            recivied_data = true;
        }

        void onTerrainBacking()
        {
            glDebug("Bathing");
            if( m_backed && !recivied_data ) return;
            if( !m_backed )
            {
                for( const auto& texture : textures_to_load )
                    loadTexture( texture );
                textures_to_load.clear();
            }
            for( unsigned y = 0; y < max_side_density+1; ++y )
                for( unsigned x = 0; x < max_side_density+1; ++x )
                    vertex_textures.push_back( getTexture( "grass" ) );
            moveToVRAM();
            recivied_data = false;
            m_backed = true;
            glDebug("End of batching");
        };

        float quadSize() const
        {
            return (float) side_size_in_meters / (float) max_side_density;
        };

        float quadCount() const
        {
            return max_side_density;
        };

      private:
        void pushVertex( TerrainSectionVertex&& v )
        {
            verticies.push_back( v );
        };

        inline auto size() const -> unsigned;

      // Data
        unsigned m_id; ///< Unique Section id
        scene::bounding_area m_area;
        static const int side_size_in_meters; ///< Size of Sections
        const unsigned   max_side_density; /// number of quads

        std::vector< TerrainSectionVertex > verticies; // ram
        std::vector< GLuint > vertex_textures;
        std::set< std::string > textures_to_load;
        std::vector< unsigned char > data; // vram
        std::map< std::string, GLuint > textures;

        GLuint vbo;
        std::unique_ptr< gl::vao > vao;
        GLuint ebo;

        bool recivied_data = false; // should Terrain prepare this data
        bool m_backed = false; // can be rendered bt opengl
    };

    constexpr auto TerrainSectionVertex::size() -> unsigned
    { return sizeof( GLuint ) + sizeof( GLfloat ) * 4; };

    inline bool Section::operator==( const Section& s )
    { return this->m_id == s.m_id; };

    auto Section::id() const -> int { return m_id; }
    auto Section::area() const -> const scene::bounding_area& { return m_area; }


    auto Section::size() const -> unsigned
    { return data.size(); };
}

#endif // !TERRAIN_SECTION_15_11_18
