/**
* @Author   Guillaume Labey
*/

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Engine/Graphics/ShaderProgram.hpp>
#include <Engine/Graphics/MeshInstance.hpp>


MeshInstance::MeshInstance(Mesh* mesh) : _mesh(mesh), _material(mesh->getMaterial()) {}

MeshInstance::MeshInstance(const MeshInstance& meshInstance)
{
    _mesh = meshInstance._mesh;
    _material = meshInstance._material;
}

MeshInstance::~MeshInstance() {}

MeshInstance& MeshInstance::operator=(const MeshInstance& meshInstance)
{
    _mesh = meshInstance._mesh;
    _material = meshInstance._material;
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
    _material = material;
}
