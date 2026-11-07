#pragma once

#include <glad/gl.h>
#include "VBO.h"


class VAO{
    public:
        GLuint ID;

        VAO();
        void LinkAttribs(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);
        void Bind();
        void Unbind();
        void Delete();
};