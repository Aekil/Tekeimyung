/**
* @Author   Guillaume Labey
*/

#pragma once

# include <vector>
# include <Engine/Graphics/Model.hpp>
# include <Engine/Graphics/Material.hpp>

class ModelInstance
{
public:
    ModelInstance(Model* model);
    ModelInstance(const ModelInstance& modelInstance);
    ~ModelInstance();

    ModelInstance& operator=(const ModelInstance& modelInstance);

    void                        draw(const ShaderProgram& shaderProgram, const glm::vec4& color, const glm::mat4 transform) const;
    const Model*                getModel() const;
    Model*                      getModel();
    std::vector<Material*>&     getMaterials();

private:
    Model*                      _model;

    // Material for each mesh
    // By default, the materials of _model are copied
    std::vector<Material*>      _materials;
};
