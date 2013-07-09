#version 330

in vec4 pos;
in vec2 texture_coord;
out vec2 texture_coord_from_vshader;

uniform mat4 ModelView;
uniform mat4 Projection;

uniform vec3 position;

void main() {
    vec4 out_pos = pos + vec4(position, 0.0);
    gl_Position = Projection * ModelView * out_pos;
    texture_coord_from_vshader = texture_coord;
}