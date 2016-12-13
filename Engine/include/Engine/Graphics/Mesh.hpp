/**
* @Author   Guillaume Labey
*/

#pragma once

# include <vector>
# include <assimp/scene.h>

# include <Engine/Graphics/Buffer.hpp>
# include <Engine/Graphics/Material.hpp>
# include <Engine/Graphics/Skeleton.hpp>

class Mesh {
public:
    Mesh();
    virtual ~Mesh();

    virtual bool                    loadFromAssimp(Skeleton& skeleton, aiMesh *mesh);

public:
    std::vector<Vertex>             vertexs;
    std::vector<GLuint>             indices;

    uint32_t                        offset;
    uint32_t                        idxOffset;

    // Material
    Material                        material;

private:
    void                            loadBones(Skeleton& skeleton, aiMesh *mesh);
    void                            addVertexBonesInfos(uint32_t vertexId, float weight, uint32_t boneId);
};
