#include <chrono>
#include <thread>
#include <glload/gl_core.hpp>
#include "common/window.hpp"
#include <iostream>
#include "GLutil/shader.hpp"
#include "GLutil/buffers.hpp"

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

namespace Peanuts{
    int Main() {
        run = true;
        Peanuts::WindowOptions windowOptions("GL test", Peanuts::Windowed(std::pair<int,int>(640,480),Peanuts::Centered()), Peanuts::OpenGLVersion(1, 4));
        auto win  = Peanuts::Window::create(windowOptions);
        EventHandler eventHandler;
        gl::ClearColor(1.0f, 0.0f, 0.0f, 0.0f);
        std::chrono::milliseconds dura( 2000 );

        std::vector<GLfloat> vertexData = {
            0.0, 0.0,
            0.5, 0.0,
            0.5, 0.5,
        
            0.0, 0.0,
            0.0, 0.5,
            -0.5, 0.5,
        
            0.0, 0.0,
            -0.5, 0.0,
            -0.5, -0.5,     

            0.0, 0.0,
            0.0, -0.5,
            0.5, -0.5,
        };

        GLutil::VertexBuffer vertexBuffer = GLutil::VertexBuffer();
        vertexBuffer.BufferData(vertexData);

        std::string vert_shader_code = (
            "#version 150\n"
            "\n"
            "in vec4 position;\n"
            "\n"
            "void main() {\n"
            "    gl_Position = position;\n"
            "}\n"
        );
        std::string frag_shader_code = (
            "#version 150\n"
            "\n"
            "out vec4 out_color;\n"
            "\n"
            "void main() {\n"
            "    out_color = vec4(0.0, 1.0, 1.0, 1.0);\n"
            "}\n"
        );
        GLutil::Program program = GLutil::Program(vert_shader_code, frag_shader_code);

        GLuint vao;
        gl::GenVertexArrays(1, &vao);
        gl::BindVertexArray(vao);

        GLint position_attribute = program.GetAttribLocation("position");
        program.use();

        // Specify how the data for position can be accessed
        gl::VertexAttribPointer(position_attribute, 2, gl::GL_FLOAT, gl::GL_FALSE, 0, 0);
        // Enable the attribute
        gl::EnableVertexAttribArray(position_attribute);

        while (run) {
            gl::Clear(gl::GL_COLOR_BUFFER_BIT);
            gl::BindVertexArray(vao);
            gl::DrawArrays(gl::GL_TRIANGLES, 0, 12);

            std::this_thread::sleep_for(dura);
            win->pumpEvents();
            while(auto event = win->pollEvent()){
                boost::apply_visitor(eventHandler, *event);
            }
            win->swapBuffers();
        }
        return 0;
    }
}