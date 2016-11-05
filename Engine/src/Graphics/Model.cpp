#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Utils/Exception.hpp"
#include "Utils/Logger.hpp"

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
        aiProcess_PreTransformVertices |
        aiProcess_GenSmoothNormals);

    if (!scene)
    {
        EXCEPT(InternalErrorException, "Failed to load model \"%s\"\nError: %s", file.c_str(), importer.GetErrorString());
        return (false);
    }

    for (uint32_t i = 0; i < scene->mNumMeshes; i++)
    {
        std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();

        mesh->loadFromAssimp(scene->mMeshes[i]);
        mesh->material.loadFromAssimp(scene->mMaterials[scene->mMeshes[i]->mMaterialIndex], file.substr(0, file.find_last_of('/')));

        _meshs.push_back(mesh);
    }

    initVertexData();
    initIndexData();
    _buffer.updateData(_vertexData, getVertexsSize(), _indexData, getIndicesSize());

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
    modelTrans = glm::translate(modelTrans, glm::vec3(_pos.x, _pos.y, _pos.z));
    GLint uniModel = shaderProgram.getUniformLocation("model");
    glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(modelTrans));

    // Bind buffer
    _buffer.bind();

    for (auto &&mesh: _meshs)
    {
        mesh->material.bind(shaderProgram);

        // Draw to screen
        glDrawElements(GL_TRIANGLES, (GLuint)mesh->indices.size(), GL_UNSIGNED_INT, BUFFER_OFFSET((GLuint)mesh->idxOffset * sizeof(GLuint)));
    }
}

void    Model::update(const glm::vec2& pos, float z)
{
    _pos.x = pos.x * 25.0f;
    _pos.y = z * 12.5f;
    _pos.z = pos.y * 25.0f;
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
