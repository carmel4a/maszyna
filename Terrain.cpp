/*
This Source Code Form is subject to the
terms of the Mozilla Public License, v.
2.0. If a copy of the MPL was not
distributed with this file, You can
obtain one at
http://mozilla.org/MPL/2.0/.
*/

#include <memory>

#include "Classes.h"
#include "Terrain.h"
#include "renderer.h" // for GfxRenderer
#include "simulation.h" // for simulation::Region
namespace Terrain
{
    Manager::Manager()
    {
        for( auto& x : terrain )
            x = std::make_unique< Terrain::Section >( 1 );
    }
    bool Manager::deserialize( cParser& input )
    {
        /*
        const auto ss_count = scene::REGION_SIDE_SECTION_COUNT;
        const auto ss_count_p = scene::REGION_SIDE_SECTION_COUNT + 1;
        const auto sub_sections = 50;
        const auto w = sub_sections+1;

        auto height_map_handle = GfxRenderer.Fetch_Texture( "height" );
        auto height_map_tex = GfxRenderer.Texture( height_map_handle );
        auto get_next = [&height_map_tex]()->float
        {
            static auto i = height_map_tex.get_data().begin();
            return (float)(((unsigned char) *(i++))/255.f);
        };

        std::vector< std::vector< float > > zs;

        zs.reserve( ss_count*ss_count );
        for( int id = 0; id < ss_count*ss_count; ++id )
        {
            zs.push_back( std::vector<float>() );
            zs.back().reserve(w*w);
            for( int id2 = 0; id2 < w*w; ++id2 )
            {
                zs.back().push_back( 0.0f );
            }
        }
        
        const auto random = []()->float{ return ((float)(rand() % 10'00)/100.f); };
        for( unsigned int sy = 0; sy < ss_count; ++sy )
            for( unsigned int sx = 0; sx < ss_count; ++sx )
                if( sy == 0   // y
                || sx == 0 ) // x
                    std::generate( zs[ sx + sy*ss_count ].begin(),
                                zs[ sx+sy*ss_count ].end(), random );
                else
                    for( int ssy = 0; ssy < w; ++ssy )
                        for( int ssx = 0; ssx < w; ++ssx )
                        {
                            if( ssx == 0 && ssy == 0 )
                            {
                                zs[sx+sy*ss_count][ssx+ssy*w] = zs[ sx-1 + (sy-1)*ss_count ][ (w-1) + (w-1)*w ];
                                continue;
                            }
                            if( ssx == 0 ) // if section x isnt 0
                            {
                                zs[sx+sy*ss_count][ssx+ssy*w] = zs[ sx-1 + sy*ss_count ][ (w-1) + ssy*w ];
                                continue;
                            }
                            if( ssy == 0 ) // if section y isnt 0
                            {
                                zs[sx+sy*ss_count][ssx+ssy*w] = zs[ sx + (sy-1)*ss_count ][ssx + (w-1)*w];
                                continue;
                            }
                            zs[sx + sy*ss_count][ssx+ssy*w] = random();
                        }

        unsigned int section_id = 0;
        state_serializer::scratch_data trash{};

        for( auto& x : simulation::Region->get_all_sections() )
        {
            const float size = scene::SECTION_SIZE / (float) sub_sections;
            const int size_of_tex = 2; // m
            const int samples_per_km = size / size_of_tex; // 2m per tex
            if( !x ) { ++section_id; continue; }
            glm::dvec3 origin = (glm::dvec3) glm::ceil( glm::vec3( (glm::vec3) x->area().center - glm::vec3( 500.0, 0.0, 500.0 ) ) * 0.001f )*1000.0;
            
            auto s_x = section_id % scene::REGION_SIDE_SECTION_COUNT;
            auto s_y = section_id / scene::REGION_SIDE_SECTION_COUNT;
            for( float yy = 0.0; yy < sub_sections; ++yy )
                for( float xx = 0.0; xx < sub_sections; ++xx )
                {
                    auto s = scene::shape_node();
                    s.get_name() = "none";
                    s.get_data().rangesquared_min = 0;
                    s.get_data().rangesquared_max = 1000'000;
                    s.get_data().lighting.ambient.r = 0.025f;
                    s.get_data().lighting.ambient.g = 0.025f;
                    s.get_data().lighting.ambient.b = 0.025f;
                    s.get_data().lighting.ambient.a = 1.0f;
                    s.get_data().lighting.diffuse.r = 0.5f;
                    s.get_data().lighting.diffuse.g = 0.5f;
                    s.get_data().lighting.diffuse.b = 0.5f;
                    s.get_data().lighting.diffuse.a = 1.0f;
                
                    s.get_data().lighting.specular.r = 0.010f;
                    s.get_data().lighting.specular.g = 0.010f;
                    s.get_data().lighting.specular.b = 0.010f;
                    s.get_data().lighting.specular.a = 1.0f;
                
                    s.get_data().material = GfxRenderer.Fetch_Material( "grass/grass04" );
                    s.get_data().translucent = false;
                    auto s1 = scene::shape_node( s );
                    world_vertex v;
                    v.normal.x   = 0.0f;
                    v.normal.y   = 1.0f;
                    v.normal.z   = 0.0f;

                    v.position.x = 0.0f + origin.x + xx*size;
                    v.position.y = zs[section_id][ xx + yy*w ];
                    v.position.z = 0.0f + origin.z + yy*size;
                    v.texture.s  = 0.0f;
                    v.texture.t  = 0.0f;
                    
                    s.get_data().vertices.emplace_back( v );
                    v.position.x = 0.0f + origin.x + xx*size;
                    v.position.y = zs[section_id][ xx + (yy+1)*w ];
                    v.position.z = size + origin.z + yy*size;
                    v.texture.s  = 0.0f;
                    v.texture.t  = 1.0f * samples_per_km;
                    s.get_data().vertices.emplace_back( v );
                    v.position.x = size + origin.x + xx*size;
                    v.position.y = zs[section_id][ xx+1 + yy*w ];
                    v.position.z = 0.0f + origin.z + yy*size;
                    v.texture.s  = 1.0f * samples_per_km;
                    v.texture.t  = 0.0f;
                    s.get_data().vertices.emplace_back( v );
                    
                    s1.get_data().vertices.emplace_back( v );
                    v.position.x = 0.0 + origin.x + xx*size;
                    v.position.y = zs[section_id][ xx + (yy+1)*w ];
                    v.position.z = size + origin.z + yy*size;
                    v.texture.s  = 0.0f;
                    v.texture.t  = 1.0f * samples_per_km;
                    s1.get_data().vertices.emplace_back( v );
                    v.position.x = size + origin.x + xx*size;
                    v.position.y = zs[section_id][ xx+1 + (yy+1)*w ];
                    v.position.z = size + origin.z + yy*size;
                    v.texture.s  = 1.0f * samples_per_km;
                    v.texture.t  = 1.0f * samples_per_km;
                    s1.get_data().vertices.emplace_back( v );
                    simulation::Region->insert( s, trash, false );
                    simulation::Region->insert( s1, trash, false );
                }
            ++section_id;
        }
        */
        return true;
    }

    Section::Section( int max_side_density )
            : max_side_density { max_side_density }
    {
        chunks.reserve( max_side_density * max_side_density );
        for( int i = 0; i < max_side_density * max_side_density; ++i )
            chunks.emplace_back( new EmptyChunk() );
    }
    
    Chunk::Chunk( ChunkTypes type )
            : type{ type }
            , center {}
            , vbo {} {}

    EmptyChunk::EmptyChunk()
            : Chunk( ChunkTypes::Empty ) {}

    NormalChunk::NormalChunk()
            : Chunk( ChunkTypes::Normal ) {}
}
