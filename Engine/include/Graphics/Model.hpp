#pragma once

# include <cstdint>
# include <list>
# include <memory>

# include "Graphics/Mesh.hpp"
# include "Graphics/Buffer.hpp"
# include "Graphics/ShaderProgram.hpp"

#define BUFFER_OFFSET(bytes) ((GLubyte*) NULL + (bytes))

class Model {
public:
    Model();
    ~Model();

    bool                        loadFromFile(const std::string &file);

    uint32_t                    getVertexsSize() const;
    uint32_t                    getIndicesSize() const;
    const std::list<std::shared_ptr<Mesh> > &getMeshs() const;
    const glm::vec3&            getPos() const;

    void                        draw(const ShaderProgram& shaderProgram) const;
    void                        update(const glm::vec2& pos, float z);

private:
    void                        initVertexData();
    void                        initIndexData();

private:
    std::list<std::shared_ptr<Mesh> >   _meshs;

    // Vertices raw buffers
    Vertex*                             _vertexData;
    GLuint*                             _indexData;

    // OpenGL Buffers
    Buffer                              _buffer;

    // Model position
    glm::vec3                           _pos;
};
