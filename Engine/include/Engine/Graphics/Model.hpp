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

    void                        draw(const ShaderProgram& shaderProgram) const;
    void                        update(const glm::vec4& color, const glm::mat4 transform);

protected:
    void                        initVertexData();
    void                        initIndexData();

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

    // Model position
    glm::vec4                           _color;
    glm::mat4                           _transform;

    // Model skeleton used for animations
    Skeleton                            _skeleton;

    aiScene*                            _scene;
    Assimp::Importer                    _importer;
    float                               _anim;
};
