#version 330

layout ( location = 0 ) in uint id;
layout ( location = 1 ) in float z;
layout ( location = 2 ) in vec3 in_normals;
layout ( location = 3 ) in mat4 MVP;

out vec3 normals;
uniform uint section_size;

void main() {
    normals = in_normals;
    gl_Position = MVP * vec4( ( int(id) / int(section_size) ) * 1000,
                              ( int(id) % int(section_size) ) * 1000,
                              z, 1 );
}
