/**
 * 
 */

#include "../renderer.h"
#include "../Terrain/Terrain.h"
#include "../Terrain/Section.h"


using Terrain::SectionsContainer;

void opengl_renderer::Render( const Terrain::SectionsContainer& active_sections )
{	
	switch (m_renderpass.draw_mode)
	{
		case rendermode::color:
		{
            std::vector< Terrain::Section* > visible_sections;
            for( const auto& id_section_pair : active_sections )
            {
                const auto& section = id_section_pair.second;
                section->onTerrainBacking();
                if( !section->backed() ) continue;
                if( m_renderpass.camera.visible( section->area() ) )
                {
                    /*
                    ::glPushMatrix();
                    auto const originoffset =
                            section->area().center
                            - m_renderpass.camera.position();
                    ::glTranslated( originoffset.x,
                                    originoffset.y,
                                    originoffset.z );
                    */
                    glUseProgram( section->getShader() );
                    glBindVertexArray( section->getVAO() );
                    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, section->getEBO() );
/*
                    glActiveTexture( GL_TEXTURE0 );
                    glBindTexture( GL_TEXTURE_2D,  )
*/

                    auto const originoffset =
                            section->area().center
                            - m_renderpass.camera.position();

                    glm::mat4x4 MVP = m_renderpass.camera.projection()
                        * m_renderpass.camera.modelview() * glm::mat4x4(
                        1.0, 0.0, 0.0, originoffset.x,
                        0.0, 1.0, 0.0, originoffset.y,
                        0.0, 0.0, 1.0, originoffset.z,
                        0.0, 0.0, 0.0,             1.0
                    );

                    GLint section_size = glGetUniformLocation( section->getShader(), "section_size" );
                    glUniform1ui( section_size, section->getSideSize() );

                    GLint u_MVP = glGetUniformLocation( section->getShader(), "MVP" );
                    glUniformMatrix4fv(u_MVP, 1, GL_FALSE, &MVP[0][0]);

                    glDrawElements( GL_TRIANGLES, section->elementsCount(), GL_UNSIGNED_INT, NULL );
                    glUseProgram( 0 );
                    glBindVertexArray( 0 );
                    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

                    // post-render cleanup
                    /*
                    ::glPopMatrix();
                    */
                }
            }
			break; 
		}
		case rendermode::shadows:
		case rendermode::pickscenery:
		case rendermode::reflections:
		case rendermode::pickcontrols:
		default: break;
    }
}
