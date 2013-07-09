#pragma once
#include <glload/gl_core.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/swizzle.hpp>
#include "program.hpp"
#include "vertexBuffer.hpp"
#include "vertexArray.hpp"
#include "texture.hpp"
#include "imageLoader.hpp"
#include "util/loadShader.hpp"

struct DirectionalLight{
    DirectionalLight(float r, float g, float b, float a):color(r,g,b), ambientIntensity(a){}
    glm::vec3 color;
    float ambientIntensity;
};

struct Crate{
    Crate();
    void draw() const;
    void projectWith(const glm::mat4& projectionMatix);

    DirectionalLight directionalLight;
private:
    gldr::Program program;
    gldr::VertexArray vao;
    gldr::indexVertexBuffer indexBuffer;
    gldr::dataVertexBuffer vertexBuffer;
    gldr::dataVertexBuffer textureCoordBuffer;
    gldr::Texture2d texture;

    GLint position_attribute;
    GLint color_attribute;
    GLint texture_coord_attribute;
    GLint modelview;
    GLint projection;
    GLint dLightColor;
    GLint dLightAmbient;

    glm::vec3 position;
    GLint positionUniform;
};