#include <chrono>
#include <thread>
#include <glload/gl_core.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include "common/window.hpp"

#include "shader.hpp"
#include "vertexBuffer.hpp"
#include "vertexArray.hpp"
#include "texture.hpp"
#include "imageLoader.hpp"

bool run;
struct EventHandler : Peanuts::genericEventHandler{
    using Peanuts::genericEventHandler::operator();
    void operator()(const Peanuts::Event::Close& event) const{
        std::cout << "Window closed!" << std::endl;
        run = false;
    }
    void operator()(const Peanuts::Event::KeyDown& event) const{
        if (event.key == Peanuts::KeyCode::Q){
            run = false;
        }
    }
};

std::string loadShader(const std::string& sourceFile){
    std::ifstream file;
    file.open(sourceFile);
    if(!file.is_open()){
        std::cerr << "Failed top open '" << sourceFile << "'" << std::endl;
        return "";
    }
    std::string slurped;
    file.seekg(0, std::ios::end);
    slurped.resize(file.tellg());
    file.seekg(0, std::ios::beg);
    file.read(&slurped[0], slurped.length());

    return slurped;
}

namespace Peanuts{
    int Main() {
        run = true;
        Peanuts::WindowOptions windowOptions("GL test", Peanuts::Windowed(std::make_pair(640,480),Peanuts::Centered()), Peanuts::OpenGLVersion(3, 1));
        auto win  = Peanuts::Window::create(windowOptions);
        EventHandler eventHandler;

        gldr::VertexArray vao;
        vao.bind();

        gl::ClearColor(1.0f, 0.0f, 0.0f, 0.0f);
        //gl::PolygonMode(gl::GL_FRONT, gl::GL_FILL);

        std::vector<GLfloat> vertexData = {
            -0.5, -0.5,
            0.5, -0.5,
            0.5, 0.5,
            -0.5, 0.5,
        };
        std::vector<GLuint> indexdata = {
            0, 1, 2,
            2, 3, 0
        };
        std::vector<GLfloat> colors = {
            1.0,0.0,0.0,
            0.0,1.0,0.0,
            0.0,0.0,1.0,
            0.0,1.0,1.0
        };
//        for(int i = 0; i < 12; ++i) {
//            float t = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
//            colors.push_back(9*(1-t)*t*t*t);
//            colors.push_back(15*(1-t)*(1-t)*t*t);
//            colors.push_back(8.5*(1-t)*(1-t)*(1-t)*t);
//            i += 3;
//        }
//        std::vector<GLfloat> textureCoord = {
//            -1.0, -1.0,
//            2.0, -1.0,
//            2.0, 2.0,
//            -1.0, 2.0,
//        };
        std::vector<GLfloat> textureCoord = {
            0.0, 1.0,
            1.0, 1.0,
            1.0, 0.0,
            0.0, 0.0,
        };

        gldr::VertexBuffer vertexBuffer;
        vertexBuffer.bufferData(vertexData);
        gldr::VertexBuffer colorBuffer;
        colorBuffer.bufferData(colors);
        gldr::VertexBuffer indexBuffer(gldr::VertexBuffer::BufferType::INDEX);
        indexBuffer.bufferData(indexdata);
        gldr::VertexBuffer textureCoordBuffer;
        textureCoordBuffer.bufferData(textureCoord);


        gldr::Texture tex;
        tex.setFiltering(gldr::Texture::FilteringDirection::Minification, gldr::Texture::FilteringMode::Linear);
        tex.setFiltering(gldr::Texture::FilteringDirection::Magnification, gldr::Texture::FilteringMode::Linear);
        {
            auto image = loadImage("resource/images/pheonixflames.png");

            tex.imageData(image.width, image.height,
                gldr::Texture::Format::RGBA,
                gldr::Texture::InternalFormat::RGBA,
                gldr::Texture::DataType::UnsignedByte,
                //rawImageData.data()
                image.data.data()
            );
        }

        gldr::Program program(loadShader("resource/shaders/basic.vert"), loadShader("resource/shaders/basic.frag"));
        GLint position_attribute = program.getAttribLocation("position");
        GLint color_attribute = program.getAttribLocation("color");
        GLint texture_coord_attribute = program.getAttribLocation("texture_coord");
        program.use();

        vertexBuffer.bind();
        gl::VertexAttribPointer(position_attribute, 2, gl::GL_FLOAT, gl::GL_FALSE, 0, 0);
        gl::EnableVertexAttribArray(position_attribute);
        colorBuffer.bind();
        gl::VertexAttribPointer(color_attribute, 3, gl::GL_FLOAT, gl::GL_FALSE, 0, 0);
        gl::EnableVertexAttribArray(color_attribute);
        textureCoordBuffer.bind();
        gl::VertexAttribPointer(texture_coord_attribute, 2, gl::GL_FLOAT, gl::GL_FALSE, 0, 0);
        gl::EnableVertexAttribArray(texture_coord_attribute);

        while (run) {
            gl::Clear(gl::GL_COLOR_BUFFER_BIT);
            vao.bind();
            gl::DrawElements(gl::GL_TRIANGLES, 6, gl::GL_UNSIGNED_INT, 0);

            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            win->pumpEvents();
            while(auto event = win->pollEvent()){
                boost::apply_visitor(eventHandler, *event);
            }
            win->swapBuffers();
        }
        return 0;
    }
}