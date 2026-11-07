#pragma once
#include <glad/gl.h>
#include <glm/glm.hpp>
#include<glad/gl.h>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include <glm/gtc/type_ptr.hpp>

class ShaderProgram{
    public:
        GLuint ID;

        ShaderProgram(const char* vertexPath, const char* fragmentPath);
        void SetVec3(const char* name, const glm::vec3& val);
        void SetMat4(const char* name, glm::mat4& value);
        void Activate();
        void Delete();
};