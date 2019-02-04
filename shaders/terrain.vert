#version 330

layout ( location = 0 ) in float z;
layout ( location = 1 ) in vec3 in_normals;

out vec3 normals;

uniform mat4 V; //debug
uniform mat4 P; //debug
// uniform mat4 VP;
uniform vec2 quad_size_and_count;
uniform vec2 position;

void main() {
    normals = in_normals;

    gl_Position = P * V
        * mat4(
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        position.x,
        z,
        position.y,
        1.0 )
        * vec4(
            float(int(gl_VertexID) / int(quad_size_and_count.y+1)) * quad_size_and_count.x,
            0.0,
            float(int(gl_VertexID) % int(quad_size_and_count.y+1)) * quad_size_and_count.x,
            1.0 );
}        
