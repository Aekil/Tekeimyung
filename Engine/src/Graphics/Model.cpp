#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Utils/Exception.hpp"
#include "Utils/Logger.hpp"
#include "Utils/Helper.hpp"

#include "Graphics/Model.hpp"


Model::Model() : _pos({0.0f, 0.0f, 0.0f}) {}

Model::~Model() {}

bool    Model::loadFromFile(const std::string &file)
{
    Assimp::Importer    importer;

    importer.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_POINT | aiPrimitiveType_LINE);

    LOG_INFO("Loading model \"%s\"", file.c_str());

    const aiScene* scene = importer.ReadFile(file,
        aiProcess_CalcTangentSpace |
        aiProcess_Triangulate |
        aiProcess_SortByPType |
        aiProcess_FixInfacingNormals |
        aiProcess_GenUVCoords |
        aiProcess_ValidateDataStructure |
        aiProcess_ImproveCacheLocality |
        aiProcess_RemoveRedundantMaterials |
        aiProcess_FindDegenerates |
        aiProcess_FindInvalidData |
        aiProcess_OptimizeMeshes |
        aiProcess_GenSmoothNormals);

    if (!scene)
    {
        EXCEPT(InternalErrorException, "Failed to load model \"%s\"\nError: %s", file.c_str(), importer.GetErrorString());
        return (false);
    }

    // Modify scene nodes transformation matrix relative to parent node
    computeSceneNodeAbsoluteTransform(scene->mRootNode);
    // Transform vertices with scene node transform matrix
    transformVertices(const_cast<aiScene*>(scene), scene->mRootNode);

    for (uint32_t i = 0; i < scene->mNumMeshes; i++)
    {
        std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();

        mesh->loadFromAssimp(_skeleton, scene->mMeshes[i]);
        mesh->material.loadFromAssimp(scene->mMaterials[scene->mMeshes[i]->mMaterialIndex], file.substr(0, file.find_last_of('/')));

        _meshs.push_back(mesh);
    }

    updateBonesTransforms(scene, scene->mRootNode);

    initVertexData();
    initIndexData();
    _buffer.updateData(_vertexData, getVertexsSize(), _indexData, getIndicesSize());
    _skeleton.updateUniformBuffer();

    return (true);
}

uint32_t    Model::getVertexsSize() const
{
   uint32_t size = 0;

    for (auto &&mesh : _meshs)
    {
        size += (uint32_t) mesh->vertexs.size();
    }

    return (size);
}

uint32_t    Model::getIndicesSize() const
{
    uint32_t size = 0;

    for (auto &&mesh : _meshs)
    {
        size += (uint32_t) mesh->indices.size();
    }

    return (size);
}

const std::list<std::shared_ptr<Mesh> > &Model::getMeshs() const
{
    return (_meshs);
}

const glm::vec3&    Model::getPos() const
{
    return (_pos);
}

void    Model::draw(const ShaderProgram& shaderProgram) const
{
    // Model matrix
    glm::mat4 modelTrans;
    modelTrans = glm::translate(modelTrans, glm::vec3(_pos.x, _pos.y, _pos.z)) * _orientation;
    GLint uniModel = shaderProgram.getUniformLocation("model");
    glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(modelTrans));

    // Bones matrices
    _skeleton.getUbo().bind(shaderProgram, "bones");

    // Bind buffer
    _buffer.bind();

    for (auto &&mesh: _meshs)
    {
        mesh->material.bind(shaderProgram);

        // Draw to screen
        glDrawElements(GL_TRIANGLES, (GLuint)mesh->indices.size(), GL_UNSIGNED_INT, BUFFER_OFFSET((GLuint)mesh->idxOffset * sizeof(GLuint)));
    }
}

void    Model::update(const glm::vec2& pos, const glm::mat4& orientation, float z)
{
    _pos.x = pos.x * 25.0f;
    _pos.y = z * 12.5f;
    _pos.z = pos.y * 25.0f;
    _orientation = orientation;
}

void    Model::initVertexData()
{
    uint32_t i = 0;
    uint32_t size = getVertexsSize();

    _vertexData = new Vertex[size];
    for (auto &&mesh : getMeshs())
    {
        auto &&vertexs = mesh->vertexs;

        mesh->offset = i;

        for (uint32_t k = 0; k < vertexs.size(); k++, i++)
        {
            _vertexData[i].pos = { vertexs[k].pos.x, vertexs[k].pos.y, vertexs[k].pos.z };
            _vertexData[i].color = { vertexs[k].color.x, vertexs[k].color.y, vertexs[k].color.z };
            _vertexData[i].normal = { vertexs[k].normal.x, vertexs[k].normal.y, vertexs[k].normal.z };
            _vertexData[i].uv = { vertexs[k].uv.x, vertexs[k].uv.y };

            std::memcpy(_vertexData[i].bonesIds, vertexs[k].bonesIds, sizeof(vertexs[k].bonesIds));
            std::memcpy(_vertexData[i].bonesWeights, vertexs[k].bonesWeights, sizeof(vertexs[k].bonesWeights));
        }
    }
}

void    Model::initIndexData()
{
    uint32_t i = 0;
    uint32_t size = getIndicesSize();

    _indexData = new GLuint[size];
    for (auto &&mesh : getMeshs())
    {
        auto &&indices = mesh->indices;

        mesh->idxOffset = i;

        for (uint32_t k = 0; k < indices.size(); k++, i++) {
            _indexData[i] = indices[k] + mesh->offset;
        }
    }
}

void    Model::transformVertices(aiScene* scene, aiNode* node)
{
    aiMatrix4x4 nodeTransform = node->mTransformation;

    // Need to multiply only if it's not an identity matrix
    if (!nodeTransform.IsIdentity())
    {
        for (uint32_t i = 0; i < node->mNumMeshes; i++)  {
            auto &&mesh = scene->mMeshes[node->mMeshes[i]];
            for (uint32_t j = 0; j < mesh->mNumVertices; j++)  {
                mesh->mVertices[j] = nodeTransform * mesh->mVertices[j];
                mesh->mNormals[j] = nodeTransform * mesh->mNormals[j];
            }
        }
    }

    for (uint32_t i = 0; i < node->mNumChildren; i++) {
        transformVertices(scene, node->mChildren[i]);
    }
}

void    Model::computeSceneNodeAbsoluteTransform(aiNode* node)
{
    if (node->mParent)    {
        node->mTransformation = node->mParent->mTransformation * node->mTransformation;
    }

    for (uint32_t i = 0; i < node->mNumChildren; i++)   {
        computeSceneNodeAbsoluteTransform(node->mChildren[i]);
    }
}

void    Model::updateBonesTransforms(const aiScene* scene, aiNode* node)
{
    Skeleton::sBone* bone = _skeleton.getBoneByName(node->mName.data);

    // The scene node is a bone
    if (bone)
    {
        glm::mat4 nodeTransform;
        glm::mat4 globalTransform;
        Helper::copyAssimpMat(node->mTransformation, nodeTransform);
        Helper::copyAssimpMat(scene->mRootNode->mTransformation, globalTransform);

        // Update bone transformation
        bone->finalTransform = nodeTransform * bone->offset;
    }
    for (uint32_t i = 0; i < node->mNumChildren; i++)   {
        updateBonesTransforms(scene, node->mChildren[i]);
    }
}
