#pragma once
#include <vector>
#include "VBO.h"
#include "VAO.h"
#include "EBO.h"
#include <glad/gl.h>

class Mesh
{
public:
    Mesh(const std::vector<float>& vertices,
         const std::vector<unsigned int>& indices);

    void Draw();
    void Delete();

    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

private:
    VAO vao;
    VBO vbo;
    EBO ebo;
    unsigned int indexCount;
};
