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
        glm::vec3 vertice;
        glm::vec2 uv;
        glm::vec3 color;

        vertice.x = mesh->mVertices[j].x;
        vertice.y = mesh->mVertices[j].y;
        vertice.z = mesh->mVertices[j].z;

        if (mesh->HasVertexColors(j))
        {
            color.x = mesh->mColors[j]->r;
            color.y = mesh->mColors[j]->g;
            color.z = mesh->mColors[j]->b;
        }
        else
            color = glm::vec3(1.0f, 1.0f, 1.0f);

        if (mesh->HasTextureCoords(0))
        {
            uv.x = mesh->mTextureCoords[0][j].x;
            uv.y = 1.0f - mesh->mTextureCoords[0][j].y;
        }
        else
            uv = glm::vec2(0, 0);

        vertex.pos = vertice;
        vertex.color = color;
        vertex.uv = uv;

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
