#pragma once

#include <glad/gl.h>
#include "ShaderClass.h"

class Texture{
public:
        GLuint ID;
        GLenum type;
        Texture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType);

        void texUnit(ShaderProgram& shader, const char* uniform, GLuint unit);
        void Bind();
        void Unbind();
        void Delete();

};