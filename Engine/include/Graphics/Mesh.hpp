#pragma once

# include <vector>
# include <assimp/scene.h>

# include "Graphics/Buffer.hpp"
# include "Graphics/Material.hpp"

class Mesh {
public:
    Mesh();
    virtual ~Mesh();

    virtual bool                    loadFromAssimp(aiMesh *mesh);

    std::vector<Vertex>             vertexs;
    std::vector<GLuint>             indices;

    uint32_t                        offset;
    uint32_t                        idxOffset;

    // Material
    Material                        material;
};
