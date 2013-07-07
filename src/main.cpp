#include <chrono>
#include <thread>
#include <glload/gl_core.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include "common/window.hpp"

#include "util/camera.hpp"
#include "util/keyBoard.hpp"
#include "crate.hpp"

bool run;
glm::mat4 modelMat, viewMat, projectionMat;
util::Camera cam;
int xCamPos = 400;
util::KeyBoard kBoard;

struct EventHandler : Peanuts::genericEventHandler{
    using Peanuts::genericEventHandler::operator();
    void operator()(const Peanuts::Event::Close& event) const{
        std::cout << "Window closed!" << std::endl;
        run = false;
    }
    void operator()(const Peanuts::Event::KeyUp& event) const { kBoard.update(event); }
    void operator()(const Peanuts::Event::KeyDown& event) const{ kBoard.update(event); }
    //void operator()(const Peanuts::Event::MouseMove& event) const{
    //    std::cout << "Processing Mouse at (" << event.x << ", " << event.y << ")\n";
    //    int deltaX = event.x - xCamPos;
    //    cam.rotateYaw(static_cast<float>(deltaX) / 0.5);
    //    xCamPos = event.x;
    //    std::cout << "deltaX = " << deltaX << ". xPos now = " << xCamPos << "\n";
    //}
    void operator()(const Peanuts::Event::WindowResize& event) const{
        std::cout << "New width : " << event.width << std::endl << "New height: " << event.height << std::endl;
        cam.aspectRatio = static_cast<float>(event.width) / static_cast<float>(event.height);
        projectionMat = cam.projection();
        gl::Viewport(0, 0, static_cast<GLsizei>(event.width), static_cast<GLsizei>(event.height));
    }
    void operator()(const Peanuts::Event::MouseDown& event) const{
        switch(event.button){
            case Peanuts::MouseButton::LEFT: std::cout << "LMB Down\n"; break;
            case Peanuts::MouseButton::MIDDLE: std::cout << "MMB Down\n"; break;
            case Peanuts::MouseButton::RIGHT: std::cout << "RMB Down\n"; break;
            case Peanuts::MouseButton::SCROLL_UP: std::cout << "scroll up Down\n"; break;
            case Peanuts::MouseButton::SCROLL_DOWN: std::cout << "scroll down Down\n"; break;
            default: break;
        }
    }
    void operator()(const Peanuts::Event::MouseUp& event) const{
        switch(event.button){
            case Peanuts::MouseButton::LEFT: std::cout << "LMB Up\n"; break;
            case Peanuts::MouseButton::MIDDLE: std::cout << "MMB Up\n"; break;
            case Peanuts::MouseButton::RIGHT: std::cout << "RMB Up\n"; break;
            case Peanuts::MouseButton::SCROLL_UP: std::cout << "scroll up Up\n"; break;
            case Peanuts::MouseButton::SCROLL_DOWN: std::cout << "scroll down Up\n"; break;
            default: break;
        }
    }
};

void update(){
    if (kBoard.isDown(Peanuts::KeyCode::ESC)){
        run = false;
    }
    if (kBoard.isDown(Peanuts::KeyCode::W)){
        cam.forward(0.1f);
    }
    if (kBoard.isDown(Peanuts::KeyCode::S)){
        cam.forward(-0.1f);
    }
    if (kBoard.isDown(Peanuts::KeyCode::A)){
        cam.pan(glm::vec2(0.1f, 0.0f));
    }
    if (kBoard.isDown(Peanuts::KeyCode::D)){
        cam.pan(glm::vec2(-0.1f, 0.0f));
    }
    if (kBoard.isDown(Peanuts::KeyCode::LEFT)){
        cam.rotateYaw(-5.0f);
    }
    if (kBoard.isDown(Peanuts::KeyCode::RIGHT)){
        cam.rotateYaw(5.0f);
    }
    if (kBoard.isFirstDown(Peanuts::KeyCode::P)){
        std::cout << "CamPos = (" << cam.pos.x << ", " << cam.pos.y << ", " << cam.pos.z << ")\n";
    }
}

namespace Peanuts{
    int Main() {
        run = true;
        //Peanuts::WindowOptions windowOptions("GL test", Peanuts::Windowed(Peanuts::size(640,480), Peanuts::position(100,100)), Peanuts::OpenGLVersion(3, 1));
        Peanuts::WindowOptions windowOptions("GL test", Peanuts::Windowed(Peanuts::size(640,480), Peanuts::Centered()), Peanuts::OpenGLVersion(3, 1));
        //Peanuts::WindowOptions windowOptions("GL test", Peanuts::Windowed(Peanuts::size(640,480), Peanuts::Centered(), Peanuts::Borders::Off), Peanuts::OpenGLVersion(3, 1));
        //Peanuts::WindowOptions windowOptions("GL test", Peanuts::Windowed(Peanuts::Maximised()), Peanuts::OpenGLVersion(3, 1));
        //Peanuts::WindowOptions windowOptions("GL test", Peanuts::Windowed(Peanuts::Maximised(), Peanuts::Borders::Off), Peanuts::OpenGLVersion(3, 1));
        //Peanuts::WindowOptions windowOptions("GL test", Peanuts::FullScreen(), Peanuts::OpenGLVersion(3, 1));
        auto win  = Peanuts::Window::create(windowOptions);
        EventHandler eventHandler;

        gl::ClearColor(1.0f, 0.0f, 0.0f, 0.0f);
        gl::Enable(gl::GL_DEPTH_TEST);
        gl::DepthFunc(gl::GL_LEQUAL);
        gl::Enable(gl::GL_CULL_FACE);
        gl::CullFace(gl::GL_BACK);
        gl::PolygonMode(gl::GL_FRONT, gl::GL_FILL);

        gldr::VertexArray vao;
        vao.bind();
        gldr::Texture2d tex;
        gldr::Program program;
        {
            gldr::VertexShader vertexShader(util::loadShader("resource/shaders/basic.vert"));
            gldr::FragmentShader fragmentShader(util::loadShader("resource/shaders/basic.frag"));
            program.attach(vertexShader, fragmentShader);
            program.link();
        }
        GLint modelview, projection;

        
        gldr::indexVertexBuffer indexBuffer;
        gldr::dataVertexBuffer vertexBuffer;
        gldr::dataVertexBuffer colorBuffer;
        gldr::dataVertexBuffer textureCoordBuffer;
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

            tex.setFiltering(gldr::textureOptions::FilterDirection::Minification, gldr::textureOptions::FilterMode::Linear);
            tex.setFiltering(gldr::textureOptions::FilterDirection::Magnification, gldr::textureOptions::FilterMode::Linear);
            auto image = loadImage("resource/images/pheonixflames.png");
            tex.imageData(image.width, image.height,
                gldr::textureOptions::Format::RGBA,
                gldr::textureOptions::InternalFormat::RGB,
                gldr::textureOptions::DataType::UnsignedByte,
                image.data.data()
            );

            program.use();
            GLint position_attribute = program.getAttribLocation("position");
            GLint color_attribute = program.getAttribLocation("color");
            GLint texture_coord_attribute = program.getAttribLocation("texture_coord"); 
            modelview = program.getUniformLocation("ModelView");
            projection = program.getUniformLocation("Projection");

            vertexBuffer.bind();
            gl::VertexAttribPointer(position_attribute, 3, gl::GL_FLOAT, gl::GL_FALSE, 0, 0);
            gl::EnableVertexAttribArray(position_attribute);
            colorBuffer.bind();
            gl::VertexAttribPointer(color_attribute, 3, gl::GL_FLOAT, gl::GL_FALSE, 0, 0);
            gl::EnableVertexAttribArray(color_attribute);
            textureCoordBuffer.bind();
            gl::VertexAttribPointer(texture_coord_attribute, 2, gl::GL_FLOAT, gl::GL_FALSE, 0, 0);
            gl::EnableVertexAttribArray(texture_coord_attribute);
        }

        projectionMat = cam.projection();
        cam.pos = glm::vec3(0.0f, 0.0f, -2.0f);
        cam.dir = glm::vec3(0.0f, 0.0f, 1.0f);

        Crate crate;

        while (run) {
            gl::Clear(gl::GL_COLOR_BUFFER_BIT);
            gl::Clear(gl::GL_DEPTH_BUFFER_BIT);
            gl::UniformMatrix4fv(modelview, 1, gl::GL_FALSE, glm::value_ptr(cam.modelView()));
            gl::UniformMatrix4fv(projection, 1, gl::GL_FALSE, glm::value_ptr(projectionMat));
            vao.bind();
            tex.bind();
            program.use();
            gl::DrawElements(gl::GL_TRIANGLES, 6, gl::GL_UNSIGNED_INT, 0);
            crate.projectWith(projectionMat);
            crate.draw();

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            win->pumpEvents();
            while(auto event = win->pollEvent()){
                boost::apply_visitor(eventHandler, *event);
            }
            win->swapBuffers();
            update();
        }
        return 0;
    }
}