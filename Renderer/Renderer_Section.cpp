/**
 * 
 */

#include "../renderer.h"
#include "../Terrain/Terrain.h"
#include "../Terrain/Section.h"

#ifndef NDEBUG
    #include "gl/error.h"
#endif // !NDEBUG

using Terrain::SectionsContainer;

void opengl_renderer::Render( const Terrain::SectionsContainer& activeSections )
{	
	switch (m_renderpass.draw_mode)
	{
		case rendermode::color:
		{
            glDebug("Terrain");
            std::vector< Terrain::Section* > visible_sections;
            for( const auto& id_section_pair : activeSections )
            {
                glDebug("In terrain loop.");
                const auto& section = id_section_pair.second;
                section->onTerrainBacking();
                if( !section->backed() ) continue;
                if( m_renderpass.camera.visible( section->area() ) )
                {
                    glDebug("Section is visible.");
                    /*
                    ::glPushMatrix();
                    auto const originoffset =
                            section->area().center
                            - m_renderpass.camera.position();
                    ::glTranslated( originoffset.x,
                                    originoffset.y,
                                    originoffset.z );
                    */
                    glCall(glUseProgram( section->getShader() ));
                    glCall(glBindVertexArray( section->getVAO() ));
                    glCall(glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, section->getEBO() ));
                    glDebug("After set state.");
                    /*
                    glActiveTexture( GL_TEXTURE0 );
                    glBindTexture( GL_TEXTURE_2D,  )
                    */
                    /*
                    auto const originoffset =
                            section->area().center
                            - m_renderpass.camera.position();
                    */
                    auto const originoffset =
                            section->area().center;

                    glm::mat4 VP = m_renderpass.camera.projection()
                            * m_renderpass.camera.modelview();

                    glCall(GLint quad_size_and_count = glGetUniformLocation( section->getShader(), "quad_size_and_count" ));
                    glCall(glUniform2f( quad_size_and_count, section->quadSize(), section->quadCount() ) );

                    glCall(GLint u_P = glGetUniformLocation( section->getShader(), "P" ));
                    glCall(glUniformMatrix4fv(u_P, 1, GL_FALSE, &m_renderpass.camera.projection()[0][0]));

                    glCall(GLint u_V = glGetUniformLocation( section->getShader(), "V" ));
                    glCall(glUniformMatrix4fv(u_V, 1, GL_FALSE, &m_renderpass.camera.modelview()[0][0]));

                    /*

                    glCall(GLint u_VP = glGetUniformLocation( section->getShader(), "VP" ));
                    glCall(glUniformMatrix4fv(u_VP, 1, GL_FALSE, &VP[0][0]));

*/

                    glCall(GLint u_position = glGetUniformLocation( section->getShader(), "position" ));
                    glCall(glUniform2f(u_position, originoffset.x, originoffset.z ));

                    glDebug("After set uniforms.");

                    glCall(glDrawElements( GL_TRIANGLES, section->elementsCount(), GL_UNSIGNED_INT, NULL ));
                    glDebug("Drawcall.");
                    glCall(glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 ));
                    glCall(glBindVertexArray( 0 ));
                    glCall(glUseProgram( 0 ));

                    glDebug("Reset state.");

                    // post-render cleanup
                    /*
                    ::glPopMatrix();
                    */
                }
            }
            glDebug("End terrain");
			break; 
		}
		case rendermode::shadows:
		case rendermode::pickscenery:
		case rendermode::reflections:
		case rendermode::pickcontrols:
		default: break;
    }
}
