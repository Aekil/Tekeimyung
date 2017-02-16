/**
* @Author   Guillaume Labey
*/

#pragma once

# include <vector>
# include <Engine/Graphics/Mesh.hpp>
# include <Engine/Graphics/Material.hpp>

class MeshInstance
{
public:
    MeshInstance(Mesh* mesh);
    MeshInstance(const MeshInstance& MeshInstance);
    ~MeshInstance();

    MeshInstance& operator=(const MeshInstance& meshInstance);

    const Mesh*                 getMesh() const;
    Mesh*                       getMesh();
    const Material*             getMaterial() const;
    Material*                   getMaterial();

    void                        setMaterial(Material* material);

private:
    Mesh*                       _mesh;
    Material*                   _material;
};
