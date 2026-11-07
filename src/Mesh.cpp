#include "headers/Mesh.h"
#include <glad/gl.h>

Mesh::Mesh(const std::vector<float>& vertices,
           const std::vector<unsigned int>& indices)
    : vao(),
      vbo(vertices.data(), static_cast<GLsizeiptr>(vertices.size() * sizeof(float))),
      ebo(indices.data(), static_cast<GLsizeiptr>(indices.size() * sizeof(unsigned int))),
      indexCount(static_cast<unsigned int>(indices.size()))
{
    vao.Bind();
    vbo.Bind();
    ebo.Bind();

    vao.LinkAttribs(vbo, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
    vao.LinkAttribs(vbo, 1, 3, GL_FLOAT, 6*sizeof(float),
                (void*)(3 * sizeof(float)));
    vao.Unbind();
    vbo.Unbind();
    ebo.Unbind();
}

void Mesh::Draw()
{
    vao.Bind();
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
}

void Mesh::Delete()
{
    vao.Delete();
    vbo.Delete();
    ebo.Delete();
}