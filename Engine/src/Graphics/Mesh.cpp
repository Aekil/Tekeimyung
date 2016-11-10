#include <cstring>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

#include "Utils/Helper.hpp"
#include "Utils/Debug.hpp"

#include "Graphics/Mesh.hpp"

Mesh::Mesh() : offset(0), idxOffset(0)
{}

Mesh::~Mesh()
{}

bool Mesh::loadFromAssimp(Skeleton& skeleton, aiMesh *mesh) {
    for (uint32_t j = 0; j < mesh->mNumVertices; j++)
    {
        Vertex vertex;
        glm::vec3 pos;
        glm::vec3 normal;
        glm::vec3 color;
        glm::vec2 uv;

        pos.x = mesh->mVertices[j].x;
        pos.y = mesh->mVertices[j].y;
        pos.z = mesh->mVertices[j].z;

        // Color
        if (mesh->HasVertexColors(j))
        {
            color.x = mesh->mColors[j]->r;
            color.y = mesh->mColors[j]->g;
            color.z = mesh->mColors[j]->b;
        }
        else
            color = glm::vec3(1.0f, 1.0f, 1.0f);

        // Uv
        if (mesh->HasTextureCoords(0))
        {
            uv.x = mesh->mTextureCoords[0][j].x;
            uv.y = -mesh->mTextureCoords[0][j].y;
        }
        else
            uv = glm::vec2(0.0f, 0.0f);

        // Normals
        if (mesh->HasNormals())
        {
            normal.x = mesh->mNormals[j].x;
            normal.y = mesh->mNormals[j].y;
            normal.z = mesh->mNormals[j].z;
        }
        else
            normal = glm::vec3(1.0f, 1.0f, 1.0f);


        vertex.pos = pos;
        vertex.color = color;
        vertex.normal = normal;
        vertex.uv = uv;
        std::memset(vertex.bonesIds, 0, sizeof(vertex.bonesIds));
        std::memset(vertex.bonesWeights, 0, sizeof(vertex.bonesWeights));

        vertexs.push_back(vertex);
    }

    for (uint32_t j = 0; j < mesh->mNumFaces; j++)
    {
        auto &&face = mesh->mFaces[j];
        for (int k = 0; k < 3; k++) {
            indices.push_back(face.mIndices[k]);
        }
    }

    loadBones(skeleton, mesh);

    return (true);
}

void    Mesh::loadBones(Skeleton& skeleton, aiMesh *mesh)
{
    if (!mesh->HasBones())
        return;

    // Iterate over all bones of the mesh
    for (uint32_t i = 0; i < mesh->mNumBones; i++)
    {
        auto &&meshBone = mesh->mBones[i];
        Skeleton::sBone* bone = skeleton.getBoneByName(meshBone->mName.C_Str());

        // the bone does not exist in the model skeleton
        if (!bone)
        {
            glm::mat4 boneOffset;
            Helper::copyAssimpMat(meshBone->mOffsetMatrix, boneOffset);
            skeleton.addBone(meshBone->mName.C_Str(), boneOffset);

            bone = skeleton.getBoneByName(meshBone->mName.C_Str());
            ASSERT(bone != nullptr, "The bone should not be null");
        }

        // Add bone informations on vertices
        for (uint32_t j = 0; j = meshBone->mNumWeights; j++)
        {
            addVertexBonesInfos(meshBone->mWeights[j].mVertexId, meshBone->mWeights[j].mWeight, bone->id);
        }
    }
}

void    Mesh::addVertexBonesInfos(uint32_t vertexId, float weight, uint32_t boneId)
{
    uint32_t i;

    for (i = 0; i < BONES_PER_VERTEX; i++)
    {
        // Vertex weight not added yet, add new vertex bone informations
        if (vertexs[vertexId].bonesWeights[i] == 0.0f)
        {
            vertexs[vertexId].bonesWeights[i] = weight;
            vertexs[vertexId].bonesIds[i] = boneId;
            break;
        }
    }

    ASSERT(i != BONES_PER_VERTEX, "Vertex has got more bone informations than supported");
}
