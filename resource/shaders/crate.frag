#version 330

struct DirectionalLight{
     vec3 color;
     float ambientIntensity;
};

in vec2 texture_coord_from_vshader;

uniform sampler2D texture_sampler;
uniform DirectionalLight directionalLight;

out vec4 out_color;

void main() {
    out_color = texture(texture_sampler, texture_coord_from_vshader) 
                * vec4(directionalLight.color, 1.0f) 
                * directionalLight.ambientIntensity;
}