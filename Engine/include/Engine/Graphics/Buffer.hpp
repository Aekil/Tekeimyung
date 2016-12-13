/**
* @Author   Guillaume Labey
*/

#pragma once

#define BONES_PER_VERTEX 8

#include <cstdint>
#include <GL/glew.h>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

typedef struct
{
    glm::vec3       pos;
    glm::vec3       color;
    glm::vec3       normal;
    glm::vec2       uv;
    int             bonesIds[BONES_PER_VERTEX];
    float           bonesWeights[BONES_PER_VERTEX];
}                   Vertex;

class Buffer
{
public:
    Buffer();
    ~Buffer();

    void                                    bind() const;
    void                                    updateData(Vertex *vertices, int verticesNb, GLuint *indices, int indicesNb);
    int                                     getIndicesNb() const;
    int                                     getVerticesNb() const;

private:
    // Vertex array buffer
    GLuint                                  _VAO;

    // Vertex buffer
    GLuint                                  _VBO;
    int                                     _verticesNb;

    // Index buffer
    GLuint                                  _EBO;
    int                                     _indicesNb;
};
