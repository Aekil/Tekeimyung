#include <iostream>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

#include "Graphics/Mesh.hpp"

Mesh::Mesh() : offset(0), idxOffset(0)
{}

Mesh::~Mesh()
{}

bool Mesh::loadFromAssimp(aiMesh *mesh) {
    for (uint32_t j = 0; j < mesh->mNumVertices; j++)
    {
        Vertex vertex;
        glm::vec3 pos;
        glm::vec3 normal;
        glm::vec3 color;

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

        vertexs.push_back(vertex);
    }

    for (uint32_t j = 0; j < mesh->mNumFaces; j++)
    {
        auto &&face = mesh->mFaces[j];
        for (int k = 0; k < 3; k++) {
            indices.push_back(face.mIndices[k]);
        }
    }

    return (true);
}
