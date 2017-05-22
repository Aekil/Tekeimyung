/**
* @Author   Guillaume Labey
*/

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Engine/Graphics/ShaderProgram.hpp>
#include <Engine/Graphics/MeshInstance.hpp>


MeshInstance::MeshInstance(Mesh* mesh) : _mesh(mesh)
{
    setMaterial(mesh->getMaterial());
}

MeshInstance::MeshInstance(const MeshInstance& meshInstance)
{
    _mesh = meshInstance._mesh;
    setMaterial(meshInstance._material);
}

MeshInstance::~MeshInstance()
{
    if (_material)
    {
        delete _material;
    }
}

MeshInstance& MeshInstance::operator=(const MeshInstance& meshInstance)
{
    _mesh = meshInstance._mesh;
    setMaterial(meshInstance._material);
    return (*this);
}

const Mesh*    MeshInstance::getMesh() const
{
    return (_mesh);
}

Mesh*  MeshInstance::getMesh()
{
    return (_mesh);
}

const Material*    MeshInstance::getMaterial() const
{
    return (_material);
}

Material*  MeshInstance::getMaterial()
{
    return (_material);
}

void    MeshInstance::setMaterial(Material* material)
{
    if (!material)
    {
        return;
    }

    // Copy material
    if (_material)
    {
        *_material = *material;
    }
    // Create new material
    else
    {
        _material = new Material(*material);
    }
}
