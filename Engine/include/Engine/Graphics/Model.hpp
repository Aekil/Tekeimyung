/**
* @Author   Guillaume Labey
*/

#pragma once

# include <cstdint>
# include <vector>
# include <memory>
# include <glm/gtx/quaternion.hpp>
# include <assimp/Importer.hpp>

# include <Engine/Graphics/Mesh.hpp>
# include <Engine/Graphics/Buffer.hpp>
# include <Engine/Graphics/ShaderProgram.hpp>
# include <Engine/Graphics/Skeleton.hpp>
# include <Engine/Utils/Resource.hpp>

#define BUFFER_OFFSET(bytes) ((GLubyte*) NULL + (bytes))

class Model: public Resource {
public:
    Model();
    ~Model();

    bool                        loadFromFile(const std::string &file);

    uint32_t                    getVertexsSize() const;
    uint32_t                    getIndicesSize() const;
    const std::vector<std::shared_ptr<Mesh> > &getMeshs() const;
    const glm::vec3&            getSize() const;
    const glm::vec3&            getMin() const;
    const glm::vec3&            getMax() const;

    void                        draw(const ShaderProgram& shaderProgram, const glm::vec4& color, const glm::mat4 transform) const;

protected:
    void                        initVertexData();
    void                        initIndexData();

    void                        calculateSize();

private:
    void                        transformVertices(aiScene* scene, aiNode* node);
    void                        computeSceneNodeAbsoluteTransform(aiNode* node);
    const aiNodeAnim*           getNodeAnim(const aiScene* scene, const std::string& name);
    void                        updateBonesTransforms(const aiScene* scene, aiNode* node, const glm::mat4& parentTransform, float test);

protected:
    std::vector<std::shared_ptr<Mesh> >   _meshs;

    // Vertices raw buffers
    Vertex*                             _vertexData;
    GLuint*                             _indexData;

    // OpenGL Buffers
    Buffer                              _buffer;

    // Model skeleton used for animations
    Skeleton                            _skeleton;

    aiScene*                            _scene;
    Assimp::Importer                    _importer;
    float                               _anim;

    glm::vec3                           _size;
    glm::vec3                           _min;
    glm::vec3                           _max;

    GLuint                              _primitiveType;
};
