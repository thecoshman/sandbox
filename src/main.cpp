#include <chrono>
#include <thread>
#include <glload/gl_core.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include "common/window.hpp"

#include "util/camera.hpp"
#include "program.hpp"
#include "vertexBuffer.hpp"
#include "vertexArray.hpp"
#include "texture.hpp"
#include "imageLoader.hpp"

bool run;
glm::mat4 modelMat, viewMat, projectionMat;
util::Camera cam;

struct EventHandler : Peanuts::genericEventHandler{
    using Peanuts::genericEventHandler::operator();
    void operator()(const Peanuts::Event::Close& event) const{
        std::cout << "Window closed!" << std::endl;
        run = false;
    }
    void operator()(const Peanuts::Event::KeyDown& event) const{
        if (event.key == Peanuts::KeyCode::ESC){
            run = false;
        }
        if (event.key == Peanuts::KeyCode::W){
            cam.forward(0.1f);
        }
        if (event.key == Peanuts::KeyCode::S){
            cam.forward(-0.1f);
        }
        if (event.key == Peanuts::KeyCode::A){
            cam.pan(glm::vec2(0.1f, 0.0f));
        }
        if (event.key == Peanuts::KeyCode::D){
            cam.pan(glm::vec2(-0.1f, 0.0f));
        }
        if (event.key == Peanuts::KeyCode::LEFT){
            cam.rotateYaw(-5.0f);
        }
        if (event.key == Peanuts::KeyCode::RIGHT){
            cam.rotateYaw(5.0f);
        }
    }
    void operator()(const Peanuts::Event::WindowResize& event) const{
        std::cout << "New width : " << event.width << std::endl << "New height: " << event.height << std::endl;
        cam.aspectRatio = static_cast<float>(event.width) / static_cast<float>(event.height);
        projectionMat = cam.projection();
    }
    void operator()(const Peanuts::Event::FocusLoose& event) const{
        std::cout << "Focus lost :(" << std::endl;
    }
    void operator()(const Peanuts::Event::FocusGain& event) const{
        std::cout << "Focus gained :)" << std::endl;
    }
    void operator()(const Peanuts::Event::MouseMove& event) const{
        std::cout << "Mouse (" << event.x << ", " << event.y << ")" << std::endl;
    }
    void operator()(const Peanuts::Event::MouseDown& event) const{
        switch(event.button){
            case Peanuts::MouseButton::LEFT: std::cout << "LMB Down" << std::endl; break;
            case Peanuts::MouseButton::MIDDLE: std::cout << "MMB Down" << std::endl; break;
            case Peanuts::MouseButton::RIGHT: std::cout << "RMB Down" << std::endl; break;
            case Peanuts::MouseButton::SCROLL_UP: std::cout << "scroll up Down" << std::endl; break;
            case Peanuts::MouseButton::SCROLL_DOWN: std::cout << "scroll down Down" << std::endl; break;
            default: break;
        }
    }
    void operator()(const Peanuts::Event::MouseUp& event) const{
        switch(event.button){
            case Peanuts::MouseButton::LEFT: std::cout << "LMB Up" << std::endl; break;
            case Peanuts::MouseButton::MIDDLE: std::cout << "MMB Up" << std::endl; break;
            case Peanuts::MouseButton::RIGHT: std::cout << "RMB Up" << std::endl; break;
            case Peanuts::MouseButton::SCROLL_UP: std::cout << "scroll up Up" << std::endl; break;
            case Peanuts::MouseButton::SCROLL_DOWN: std::cout << "scroll down Up" << std::endl; break;
            default: break;
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
        Peanuts::WindowOptions windowOptions("GL test", Peanuts::Windowed(Peanuts::size(640,480), Peanuts::position(100,100)), Peanuts::OpenGLVersion(3, 1));
        auto win  = Peanuts::Window::create(windowOptions);
        EventHandler eventHandler;

        gldr::VertexArray vao;
        vao.bind();

        gl::ClearColor(1.0f, 0.0f, 0.0f, 0.0f);
        //gl::PolygonMode(gl::GL_FRONT, gl::GL_FILL);

        gldr::VertexBuffer vertexBuffer;
        gldr::VertexBuffer colorBuffer;
        gldr::VertexBuffer indexBuffer(gldr::VertexBuffer::BufferType::INDEX);
        gldr::VertexBuffer textureCoordBuffer;
        {
            std::vector<GLfloat> vertexData = {
                -0.5, -0.5, -0.5,
                0.5, -0.5, -0.5,
                0.5, 0.5, -0.5,
                -0.5, 0.5, -0.5,
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
            std::vector<GLfloat> textureCoord = {
                0.0, 1.0,
                1.0, 1.0,
                1.0, 0.0,
                0.0, 0.0,
            };
            vertexBuffer.bufferData(vertexData);
            colorBuffer.bufferData(colors);
            indexBuffer.bufferData(indexdata);
            textureCoordBuffer.bufferData(textureCoord);
        }

        gldr::Texture2d tex;
        {
            tex.setFiltering(gldr::textureOptions::FilterDirection::Minification, gldr::textureOptions::FilterMode::Linear);
            tex.setFiltering(gldr::textureOptions::FilterDirection::Magnification, gldr::textureOptions::FilterMode::Linear);
            auto image = loadImage("resource/images/pheonixflames.png");
            tex.imageData(image.width, image.height,
                gldr::textureOptions::Format::RGBA,
                gldr::textureOptions::InternalFormat::RGB,
                gldr::textureOptions::DataType::UnsignedByte,
                image.data.data()
            );
        }

        gldr::Program program(loadShader("resource/shaders/basic.vert"), loadShader("resource/shaders/basic.frag"));
        program.use();
        GLint position_attribute = program.getAttribLocation("position");
        GLint color_attribute = program.getAttribLocation("color");
        GLint texture_coord_attribute = program.getAttribLocation("texture_coord"); 
        GLint modelview = program.getUniformLocation("ModelView");
        GLint projection = program.getUniformLocation("Projection");

        vertexBuffer.bind();
        gl::VertexAttribPointer(position_attribute, 3, gl::GL_FLOAT, gl::GL_FALSE, 0, 0);
        gl::EnableVertexAttribArray(position_attribute);
        colorBuffer.bind();
        gl::VertexAttribPointer(color_attribute, 3, gl::GL_FLOAT, gl::GL_FALSE, 0, 0);
        gl::EnableVertexAttribArray(color_attribute);
        textureCoordBuffer.bind();
        gl::VertexAttribPointer(texture_coord_attribute, 2, gl::GL_FLOAT, gl::GL_FALSE, 0, 0);
        gl::EnableVertexAttribArray(texture_coord_attribute);

        projectionMat = cam.projection();
        cam.pos = glm::vec3(0.0f, 0.0f, -2.0f);
        cam.dir = glm::vec3(0.0f, 0.0f, 1.0f);

        while (run) {
            gl::Clear(gl::GL_COLOR_BUFFER_BIT);
            gl::UniformMatrix4fv(modelview, 1, gl::GL_FALSE, glm::value_ptr(cam.modelView()));
            gl::UniformMatrix4fv(projection, 1, gl::GL_FALSE, glm::value_ptr(projectionMat));
            vao.bind();
            gl::DrawElements(gl::GL_TRIANGLES, 6, gl::GL_UNSIGNED_INT, 0);

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            win->pumpEvents();
            while(auto event = win->pollEvent()){
                boost::apply_visitor(eventHandler, *event);
            }
            win->swapBuffers();
        }
        return 0;
    }
}