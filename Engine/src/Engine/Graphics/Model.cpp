#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Engine/Utils/Exception.hpp>
#include <Engine/Utils/Logger.hpp>
#include <Engine/Utils/Helper.hpp>

#include <Engine/Graphics/Model.hpp>


Model::Model() : _pos({0.0f, 0.0f, 0.0f}), _scale({1.0f, 1.0f, 1.0f}), _anim(0), _color({1.0f, 1.0f, 1.0f, 1.0f}) {}

Model::~Model() {}

bool    Model::loadFromFile(const std::string &file)
{
    _importer.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_POINT | aiPrimitiveType_LINE);

    LOG_INFO("Loading model \"%s\"", file.c_str());

    const aiScene* scene = _importer.ReadFile(file,
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
        EXCEPT(InternalErrorException, "Failed to load model \"%s\"\nError: %s", file.c_str(), _importer.GetErrorString());
        return (false);
    }



    // Transform vertices with scene node transform matrix
    //if (!scene->HasAnimations())
    //{
        // Modify scene nodes transformation matrix relative to parent node
        computeSceneNodeAbsoluteTransform(scene->mRootNode);
        transformVertices(_scene, scene->mRootNode);
    //}

    std::cout << scene->mNumMeshes << " meshes for " << file << std::endl;
    for (uint32_t i = 0; i < scene->mNumMeshes; i++)
    {
        std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();

        mesh->loadFromAssimp(_skeleton, scene->mMeshes[i]);
        mesh->material.loadFromAssimp(scene->mMaterials[scene->mMeshes[i]->mMaterialIndex], file.substr(0, file.find_last_of('/')));

        _meshs.push_back(mesh);
    }


/*    if (scene->HasAnimations())
    {
        std::cout << "FOUND " << scene->mNumAnimations << " animations" << std::endl;
        std::cout << "FIRST: " << scene->mAnimations[0]->mName.data << std::endl;
        updateBonesTransforms(scene, scene->mRootNode, glm::mat4(1.0), 2.0f);
    }*/

    initVertexData();
    initIndexData();
    _buffer.updateData(_vertexData, getVertexsSize(), _indexData, getIndicesSize());
    //_skeleton.updateUniformBuffer();

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

const std::vector<std::shared_ptr<Mesh> > &Model::getMeshs() const
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
    modelTrans = glm::scale(modelTrans, _scale);
    GLint uniModel = shaderProgram.getUniformLocation("model");
    glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(modelTrans));

    // Model color
    GLint colorModel = shaderProgram.getUniformLocation("modelColor");
    glUniform4f(colorModel, _color.x, _color.y, _color.z,_color.w);

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

void    Model::update(const glm::vec4& color, const glm::vec3& pos, const glm::vec3& scale, const glm::mat4& orientation)
{
    _color = color;
    update(pos, scale, orientation);

}

void    Model::update(const glm::vec3& pos, const glm::vec3& scale, const glm::mat4& orientation)
{
    //uint32_t i = 0;
    _pos = pos;
/*    _pos.x = pos.x * 25.0f;
    _pos.y = z * 12.5f;
    _pos.z = pos.y * 25.0f;*/
    _scale = scale;
    _orientation = orientation;

   /* if (_skeleton.getBones().size() <= 0)
        return;

    if (_anim >= 32.0f)
        _anim = 0.0f;
    updateBonesTransforms(_scene, _scene->mRootNode, glm::mat4(1.0), _anim);
    for (auto &&mesh : getMeshs())
    {
        auto &&vertexs = mesh->vertexs;

        for (uint32_t k = 0; k < vertexs.size(); k++, i++)
        {
            glm::mat4 trans(1.0);
            for (uint32_t j = 0; j < BONES_PER_VERTEX; j++)
            {
               Skeleton::sBone* bone = _skeleton.getBoneById(vertexs[k].bonesIds[j]);
                trans = trans + (bone->finalTransform * vertexs[k].bonesWeights[j]);
            }
            glm::vec4 pos = trans * glm::vec4(vertexs[k].pos, 1.0);
            _vertexData[i].pos = glm::vec3(pos.x, pos.y, pos.z);
        }
    }
    _anim++;

    _buffer.updateData(_vertexData, getVertexsSize(), _indexData, getIndicesSize());*/
    //_skeleton.updateUniformBuffer();

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
            //vertexs[k].pos = glm::vec3(pos.x, pos.y, pos.z);
            /*if (_skeleton.getBones().size() > 0)
            {
                glm::mat4 trans(1.0);
                for (uint32_t j = 0; j < BONES_PER_VERTEX; j++)
                {
                   Skeleton::sBone* bone = _skeleton.getBoneById(vertexs[k].bonesIds[j]);
                    trans = trans + (bone->finalTransform * vertexs[k].bonesWeights[j]);
                }
                glm::vec4 pos = trans * glm::vec4(vertexs[k].pos, 1.0);
                _vertexData[i].pos = { pos.x, pos.y, pos.z };
            }
            else*/
                _vertexData[i].pos = { vertexs[k].pos.x, vertexs[k].pos.y, vertexs[k].pos.z };
                _vertexData[i].color = { vertexs[k].color.x, vertexs[k].color.y, vertexs[k].color.z };
                _vertexData[i].normal = { vertexs[k].normal.x, vertexs[k].normal.y, vertexs[k].normal.z };
                _vertexData[i].uv = { vertexs[k].uv.x, vertexs[k].uv.y };

            //std::memcpy(_vertexData[i].bonesIds, vertexs[k].bonesIds, sizeof(vertexs[k].bonesIds));
            //std::memcpy(_vertexData[i].bonesWeights, vertexs[k].bonesWeights, sizeof(vertexs[k].bonesWeights));
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
        std::cout << "NOODE: " << node->mName.data << std::endl;
    if (node->mParent)    {
        node->mTransformation = node->mParent->mTransformation * node->mTransformation;
    }
std::cout << "CHILDSSSS: [" << node->mNumChildren << "]" << "  " ;
    for (uint32_t i = 0; i < node->mNumChildren; i++) {
        std::cout << node->mChildren[i]->mName.data << "  ";
    }
    std::cout << std::endl;
    for (uint32_t i = 0; i < node->mNumChildren; i++)   {
        computeSceneNodeAbsoluteTransform(node->mChildren[i]);
    }
}

const aiNodeAnim*   Model::getNodeAnim(const aiScene* scene, const std::string& name)
{
    for (uint32_t i = 0; i < scene->mAnimations[0]->mNumChannels; i++)
    {
        if (name == scene->mAnimations[0]->mChannels[i]->mNodeName.data)
            return (scene->mAnimations[0]->mChannels[i]);
    }
    return (nullptr);
}

uint32_t FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
    for (uint32_t i = 0 ; i < pNodeAnim->mNumRotationKeys - 1 ; i++)
    {
        if (AnimationTime < (float)pNodeAnim->mRotationKeys[i + 1].mTime)
        {
            return i;
        }
    }

    ASSERT(0, "qsdposkdqopsdqposdk");
}

void calcInterpolatedRotation(glm::quat &out, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
    aiQuaternion rotation;
    // we need at least two values to interpolate...
    if (pNodeAnim->mNumRotationKeys == 1) {
        rotation = pNodeAnim->mRotationKeys[0].mValue;
        Helper::copyAssimpQuat(rotation, out);
        return;
    }

    uint32_t RotationIndex = FindRotation(AnimationTime, pNodeAnim);
    uint32_t NextRotationIndex = (RotationIndex + 1);
    ASSERT(NextRotationIndex < pNodeAnim->mNumRotationKeys, "lolqsdqds");
    float DeltaTime = pNodeAnim->mRotationKeys[NextRotationIndex].mTime - pNodeAnim->mRotationKeys[RotationIndex].mTime;
    float Factor = (AnimationTime - (float)pNodeAnim->mRotationKeys[RotationIndex].mTime) / DeltaTime;
    ASSERT(Factor >= 0.0f && Factor <= 1.0f, "lol");
    const aiQuaternion& StartRotationQ = pNodeAnim->mRotationKeys[RotationIndex].mValue;
    const aiQuaternion& EndRotationQ = pNodeAnim->mRotationKeys[NextRotationIndex].mValue;
    aiQuaternion::Interpolate(rotation, StartRotationQ, EndRotationQ, Factor);
    rotation = rotation.Normalize();
    Helper::copyAssimpQuat(rotation, out);
}

void    Model::updateBonesTransforms(const aiScene* scene, aiNode* node, const glm::mat4& parentTransform, float test)
{
    glm::mat4 nodeTransform;
    Helper::copyAssimpMat(node->mTransformation, nodeTransform);

    std::string nodeName = node->mName.data;
    auto nodeAnim = getNodeAnim(scene, node->mName.data);
    if (!nodeAnim)
        nodeAnim = getNodeAnim(scene, std::string(node->mName.data) + "_$AssimpFbx$_Rotation");
    if (nodeAnim)
    {
        Assimp::Interpolator<aiQuaternion> slerp;
        Assimp::Interpolator<aiVector3D> lerp;
        glm::vec3 pos;
        glm::quat rotation;
        glm::vec3 scale(1.0);

        aiQuaternion aiRotation;
        aiVector3D aiTranslation;
        aiVector3D aiScale;

        if ((uint32_t)test > nodeAnim->mNumRotationKeys - 1)
            aiRotation = nodeAnim->mRotationKeys[nodeAnim->mNumRotationKeys - 1].mValue;
        else
            aiRotation = nodeAnim->mRotationKeys[(int)test].mValue;
        if ((uint32_t)test > nodeAnim->mNumPositionKeys - 1)
            aiTranslation = nodeAnim->mPositionKeys[nodeAnim->mNumPositionKeys - 1].mValue;
        else
            aiTranslation = nodeAnim->mPositionKeys[(int)test].mValue;
        if ((uint32_t)test > nodeAnim->mNumScalingKeys - 1)
            aiScale = nodeAnim->mScalingKeys[nodeAnim->mNumScalingKeys - 1].mValue;
        else
            aiScale = nodeAnim->mScalingKeys[(int)test].mValue;

        //calcInterpolatedRotation(rotation, test, nodeAnim);
        //lerp(aiTranslation, aiTranslation, nodeAnim->mPositionKeys[0].mValue, 0.5); // translation
        //lerp(aiScale, aiScale, nodeAnim->mScalingKeys[0].mValue, 0.5); // scale*/
        //slerp(aiRotation, aiRotation, nodeAnim->mRotationKeys[36].mValue, 0.5); // rotation

        Helper::copyAssimpVec3(aiTranslation, pos);
        Helper::copyAssimpVec3(aiScale, scale);
        Helper::copyAssimpQuat(aiRotation, rotation);
        //nodeTransform = glm::translate(glm::mat4(1.0), pos) * glm::mat4_cast(rotation) * glm::scale(glm::mat4(1.0f), scale);
    }

   nodeTransform = parentTransform * nodeTransform;

    Skeleton::sBone* bone = _skeleton.getBoneByName(node->mName.data);
    if (bone)
    {
        glm::mat4 globalTransform;
        Helper::copyAssimpMat(scene->mRootNode->mTransformation, globalTransform);
        bone->finalTransform = nodeTransform * bone->offset;
    }

    for (uint32_t i = 0; i < node->mNumChildren; i++)   {
        updateBonesTransforms(scene, node->mChildren[i], nodeTransform, test);
    }
}
