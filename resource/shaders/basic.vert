#version 150

in vec4 position;
in vec4 color;
in vec2 texture_coord;
out vec4 color_from_vshader;
out vec2 texture_coord_from_vshader;

uniform mat4 ModelView;
uniform mat4 Projection;

void main() {
    gl_Position = Projection * ModelView * position;
    color_from_vshader = color;
    texture_coord_from_vshader = texture_coord;
}