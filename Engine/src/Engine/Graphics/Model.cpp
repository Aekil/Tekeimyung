/**
* @Author   Guillaume Labey
*/

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <Engine/Utils/Exception.hpp>
#include <Engine/Debug/Logger.hpp>
#include <Engine/Utils/Helper.hpp>

#include <Engine/Graphics/Model.hpp>


Model::Model() : _anim(0), _primitiveType(GL_TRIANGLES) {}

Model::~Model() {}

bool    Model::loadFromFile(const std::string &fileName)
{
    _importer.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_POINT | aiPrimitiveType_LINE);

    LOG_INFO("Loading model \"%s\"", fileName.c_str());

    const aiScene* scene = _importer.ReadFile(fileName,
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

    _scene = const_cast<aiScene*>(scene);

    if (!scene)
    {
        EXCEPT(InternalErrorException, "Failed to load model \"%s\"\nError: %s", fileName.c_str(), _importer.GetErrorString());
        return (false);
    }



    // Modify scene nodes transformation matrix relative to parent node
    computeSceneNodeAbsoluteTransform(scene->mRootNode);
    transformVertices(_scene, scene->mRootNode);

    for (uint32_t i = 0; i < scene->mNumMeshes; i++)
    {
        std::unique_ptr<Mesh> mesh = std::make_unique<Mesh>(this);

        mesh->loadFromAssimp(scene->mMeshes[i]);

        Material* material = Material::loadFromAssimp(scene->mMaterials[scene->mMeshes[i]->mMaterialIndex], fileName.substr(0, fileName.find_last_of('/')));

        if (!material)
        {
            LOG_ERROR("Failed to load material for model \"%s\"", fileName.c_str());
            return (false);
        }

        mesh->setMaterial(material);

        _meshs.push_back(std::move(mesh));
    }

    initVertexData();
    initIndexData();
    _buffer.updateData(_vertexData, getVertexsSize(), _indexData, getIndicesSize());

    calculateSize();

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

const std::vector<std::unique_ptr<Mesh> > &Model::getMeshs() const
{
    return (_meshs);
}

const Buffer&       Model::getBuffer() const
{
    return (_buffer);
}

GLuint  Model::getPrimitiveType() const
{
    return (_primitiveType);
}

bool    Model::isGeometry() const
{
    return (false);
}

const glm::vec3&    Model::getSize() const
{
    return (_size);
}

const glm::vec3&    Model::getPivot() const
{
    return (_pivot);
}

const glm::vec3&    Model::getMin() const
{
    return (_min);
}

const glm::vec3&    Model::getMax() const
{
    return (_max);
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

void    Model::calculateSize()
{
    glm::vec3 min(0.0f);
    glm::vec3 max(0.0f);

    for (auto &&mesh : getMeshs())
    {
        auto &&vertexs = mesh->vertexs;

        for (uint32_t i = 0; i < vertexs.size(); i++)
        {
            auto &&pos = vertexs[i].pos;

            // Min/Max x
            if (pos.x < min.x)
                min.x = pos.x;
            else if (pos.x > max.x)
                max.x = pos.x;

            // Min/Max y
            if (pos.y < min.y)
                min.y = pos.y;
            else if (pos.y > max.y)
                max.y = pos.y;

            // Min/Max z
            if (pos.z < min.z)
                min.z = pos.z;
            else if (pos.z > max.z)
                max.z = pos.z;
        }
    }

    _size.x = max.x - min.x;
    _size.y = max.y - min.y;
    _size.z = max.z - min.z;
    _min = min;
    _max = max;

    // Calculate the translation the model need to have his pivot point centered
    _pivot = glm::vec3(0.0f, -(_size.y / 2.0f + _min.y), 0.0f);
    _pivot += glm::vec3(-(_size.x / 2.0f + _min.x), 0.0f, 0.0f);
    _pivot += glm::vec3(0.0f, 0.0f, -(_size.z / 2.0f + _min.z));
}
