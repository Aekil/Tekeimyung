#pragma once

# include <cstdint>
# include <list>
# include <memory>
# include <glm/gtx/quaternion.hpp>
#include <assimp/Importer.hpp>

# include "Graphics/Mesh.hpp"
# include "Graphics/Buffer.hpp"
# include "Graphics/ShaderProgram.hpp"
# include "Graphics/Skeleton.hpp"

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
    void                        update(const glm::vec2& pos, const glm::vec3& scale, const glm::mat4& orientation, float z);

private:
    void                        initVertexData();
    void                        initIndexData();

    void                        transformVertices(aiScene* scene, aiNode* node);
    void                        computeSceneNodeAbsoluteTransform(aiNode* node);
    const aiNodeAnim*           getNodeAnim(const aiScene* scene, const std::string& name);
    void                        updateBonesTransforms(const aiScene* scene, aiNode* node, const glm::mat4& parentTransform, float test);

private:
    std::list<std::shared_ptr<Mesh> >   _meshs;

    // Vertices raw buffers
    Vertex*                             _vertexData;
    GLuint*                             _indexData;

    // OpenGL Buffers
    Buffer                              _buffer;

    // Model position
    glm::vec3                           _pos;
    glm::vec3                           _scale;
    // Model orientation
    glm::mat4                           _orientation;

    // Model skeleton used for animations
    Skeleton                            _skeleton;

    aiScene*                            _scene;
    Assimp::Importer                    _importer;
    float                               _anim;
};
