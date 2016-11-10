#include <GL/glew.h>

#include "Graphics/Buffer.hpp"

Buffer::Buffer()
{
    glGenBuffers(1, &_VBO);
    glGenBuffers(1, &_EBO);
    glGenVertexArrays(1, &_VAO);
}

Buffer::~Buffer()
{
    glDeleteBuffers(1, &_VBO);
    glDeleteBuffers(1, &_EBO);
    glDeleteVertexArrays(1, &_VAO);
}

void    Buffer::bind() const
{
    glBindVertexArray(_VAO);
}

void    Buffer::updateData(Vertex* vertices, int verticesNb, GLuint* indices, int indicesNb)
{
    // Store vertex and indices count
    _verticesNb = verticesNb;
    _indicesNb = indicesNb;

    // Bind Vertex Array
    glBindVertexArray(_VAO);

    // Bind VBO to GL_ARRAY_BUFFER type so that all calls to GL_ARRAY_BUFFER use VBO
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);

    // Updload vertices to VBO
    glBufferData(GL_ARRAY_BUFFER, verticesNb * sizeof(Vertex), vertices, GL_STATIC_DRAW);


    // Bind EBO to GL_ARRAY_BUFFER type so that all calls to GL_ARRAY_BUFFER use EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);

    // Updload indices to EBO
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesNb * sizeof(GLuint), indices, GL_STATIC_DRAW);


    // Tell to Opengl how to interpret vertex data
    // Vertex position
    glVertexAttribPointer(
        0, // layout(location=0)
        3, // 3 components in vec3
        GL_FLOAT, // component type
        GL_FALSE, // normalized values (between 0 and 1), GL_FALSE
        sizeof(Vertex), // size between vertices
        (GLvoid*)0);

    // Vertex color
    glVertexAttribPointer(
        1, // layout(location=1)
        3, // 3 components in vec3
        GL_FLOAT, // component type
        GL_FALSE, // normalized values (between 0 and 1), GL_FALSE
        sizeof(Vertex), // size between vertices
        (GLvoid*)(sizeof(GL_FLOAT) * 3));

    // Vertex normal
    glVertexAttribPointer(
        2, // layout(location=2)
        3, // 3 components in vec3
        GL_FLOAT, // component type
        GL_FALSE, // normalized values (between 0 and 1), GL_FALSE
        sizeof(Vertex), // size between vertices
        (GLvoid*)(sizeof(GL_FLOAT) * 6));

    // Vertex normal
    glVertexAttribPointer(
        3, // layout(location=3)
        2, // 2 components in vec2
        GL_FLOAT, // component type
        GL_FALSE, // normalized values (between 0 and 1), GL_FALSE
        sizeof(Vertex), // size between vertices
        (GLvoid*)(sizeof(GL_FLOAT) * 9));

    // Bones ids
    glVertexAttribPointer(
        4, // layout(location=4)
        BONES_PER_VERTEX, // There are BONES_PER_VERTEX bones ids
        GL_INT, // component type
        GL_FALSE, // normalized values (between 0 and 1), GL_FALSE
        sizeof(Vertex), // size between vertices
        (GLvoid*)(sizeof(GL_FLOAT) * 11));

    // Bones weights
    glVertexAttribPointer(
        5, // layout(location=5)
        BONES_PER_VERTEX, // There are BONES_PER_VERTEX bones weights
        GL_FLOAT, // component type
        GL_FALSE, // normalized values (between 0 and 1), GL_FALSE
        sizeof(Vertex), // size between vertices
        (GLvoid*)(sizeof(GL_FLOAT) * 11 + sizeof(GL_INT) * BONES_PER_VERTEX));

    // Enable vertex attributes with layout(location=0)
    glEnableVertexAttribArray(0);

    // Enable vertex attributes with layout(location=1)
    glEnableVertexAttribArray(1);

    // Enable vertex attributes with layout(location=2)
    glEnableVertexAttribArray(2);

    // Enable vertex attributes with layout(location=3)
    glEnableVertexAttribArray(3);

    // Unbind Vertex Array
    glBindVertexArray(0);
}

int     Buffer::getIndicesNb() const
{
    return _indicesNb;
}

int     Buffer::getVerticesNb() const
{
    return _verticesNb;
}
