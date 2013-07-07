#version 150

in vec2 texture_coord_from_vshader;

uniform sampler2D texture_sampler;

out vec4 out_color;

void main() {
    out_color = texture(texture_sampler, texture_coord_from_vshader);
}