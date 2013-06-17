namespace GLutil{
    struct VertexBuffer{
        VertexBuffer(){
            gl::GenBuffers(1, &vbo);
        }

        ~VertexBuffer(){
            gl::DeleteBuffersARB(1, &vbo);
        }
        void BufferData(std::vector<GLfloat> data){
            Bind();
            gl::BufferData(gl::GL_ARRAY_BUFFER, sizeof(GLfloat) * data.size(), data.data(), gl::GL_STATIC_DRAW);
        }

        void Bind(){            
            gl::BindBuffer(gl::GL_ARRAY_BUFFER, vbo);
        }

    private:
        GLuint vbo;
    };

    struct VertexArray{
        VerteArray(){
            gl::GenVertexArrays(1, &vao);
        }
        
        ~VertexArray(){
            gl::DeleteVertexArrays(1​, &vao);
        }
        void Bind(){
            gl::BindVertexArray(vao);
        }
    private:
        GLunit vao;
    };
}
