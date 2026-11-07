#pragma once


#include <glad/gl.h>

class VBO{
    public:
        GLuint ID;
        VBO(const GLfloat* vertices, GLsizeiptr size);

        void Bind();
        void Unbind();
        void Delete();
};