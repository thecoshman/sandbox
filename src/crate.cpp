#include "crate.hpp"
#include <iostream>
namespace {
    std::vector<GLfloat> vertexData = {
        -0.5, -0.5, -0.5,
        -0.5, -0.5,  0.5,
        -0.5,  0.5,  0.5,
        -0.5,  0.5, -0.5,

        -0.5, -0.5,  0.5,
         0.5, -0.5,  0.5,
         0.5,  0.5,  0.5,
        -0.5,  0.5,  0.5,

         0.5, -0.5,  0.5,
         0.5, -0.5, -0.5,
         0.5,  0.5, -0.5,
         0.5,  0.5,  0.5,

         0.5, -0.5, -0.5,
        -0.5, -0.5, -0.5,
        -0.5,  0.5, -0.5,
         0.5,  0.5, -0.5,
    };

    std::vector<GLuint> indexdata = {
         0, 1, 2,
         2, 3, 0,
         4, 5, 6, 
         6, 7, 4,
         8, 9,10,
        10,11, 8,
        12,13,14,
        14,15,12,
    };

    std::vector<GLfloat> textureCoord = {
        0.00, 0.25,
        0.25, 0.25,
        0.25, 0.50,
        0.00, 0.50,

        0.25, 0.25,
        0.50, 0.25,
        0.50, 0.50,
        0.25, 0.50,

        0.50, 0.25,
        0.75, 0.25,
        0.75, 0.50,
        0.50, 0.50,

        0.75, 0.25,
        1.00, 0.25,
        1.00, 0.50,
        0.75, 0.50,
    };
}

Crate::Crate(){
    {
        gldr::VertexShader vertexShader(util::loadShader("resource/shaders/crate.vert"));
        gldr::FragmentShader fragmentShader(util::loadShader("resource/shaders/crate.frag"));
        program.attach(vertexShader, fragmentShader);
        program.link();
    }
    vao.bind();
    vertexBuffer.bufferData(vertexData);
    indexBuffer.bufferData(indexdata);
    textureCoordBuffer.bufferData(textureCoord);

    texture.setFiltering(gldr::textureOptions::FilterDirection::Minification, gldr::textureOptions::FilterMode::Linear);
    texture.setFiltering(gldr::textureOptions::FilterDirection::Magnification, gldr::textureOptions::FilterMode::Linear);
    auto image = loadImage("resource/images/crate3.png");
    texture.imageData(image.width, image.height,
        gldr::textureOptions::Format::RGBA,
        gldr::textureOptions::InternalFormat::RGB,
        gldr::textureOptions::DataType::UnsignedByte,
        image.data.data()
    );

    program.use();
    position_attribute = program.getAttribLocation("pos");
    color_attribute = program.getAttribLocation("color");
    texture_coord_attribute = program.getAttribLocation("texture_coord"); 
    modelview = program.getUniformLocation("ModelView");
    projection = program.getUniformLocation("Projection");
    positionUniform = program.getUniformLocation("position");
    vertexBuffer.bind();
    gl::VertexAttribPointer(position_attribute, 3, gl::GL_FLOAT, gl::GL_FALSE, 0, 0);
    gl::EnableVertexAttribArray(position_attribute);
    textureCoordBuffer.bind();
    gl::VertexAttribPointer(texture_coord_attribute, 2, gl::GL_FLOAT, gl::GL_FALSE, 0, 0);
    gl::EnableVertexAttribArray(texture_coord_attribute);
    
    position.x = 0;
}

void Crate::draw() const{
    vao.bind();
    texture.bind();
    program.use();
    gl::Uniform3f(positionUniform, position.x, position.y, position.z);
    gl::DrawElements(gl::GL_TRIANGLES, 3 * 8, gl::GL_UNSIGNED_INT, 0);
}


void Crate::projectWith(const glm::mat4& projectionMatix){
    program.use();
    gl::UniformMatrix4fv(projection, 1, gl::GL_FALSE, glm::value_ptr(projectionMatix));
}