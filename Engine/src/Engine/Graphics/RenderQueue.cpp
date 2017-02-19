/**
* @Author   Guillaume Labey
*/

#include <Engine/Graphics/RenderQueue.hpp>
#include <Engine/Utils/Debug.hpp>
#include <Engine/Utils/Logger.hpp>

void    RenderQueue::addModel(ModelInstance* modelInstance, const glm::vec4& color, const glm::mat4& transform)
{
    auto& meshsInstances = modelInstance->getMeshsInstances();

    for (auto& meshInstance: meshsInstances)
    {
        Material *material = meshInstance->getMaterial();
        ASSERT(material != nullptr, "A mesh should have a material");

        sRenderableMesh renderableMesh = { meshInstance.get(), color, transform };
        if (material->_transparent)
        {
            CHECK_QUEUE_NOT_FULL(_transparentMeshsNb);
            _transparentMeshs[_transparentMeshsNb] = renderableMesh;
            ++_transparentMeshsNb;
        }
        else
        {
            CHECK_QUEUE_NOT_FULL(_opaqueMeshsNb);
            _opaqueMeshs[_opaqueMeshsNb] = renderableMesh;
            ++_opaqueMeshsNb;
        }
    }
}

void    RenderQueue::addUIModel(ModelInstance* modelInstance, const glm::vec4& color, const glm::mat4& transform)
{
    auto& meshsInstances = modelInstance->getMeshsInstances();

    for (auto& meshInstance: meshsInstances)
    {
        Material *material = meshInstance->getMaterial();
        ASSERT(material != nullptr, "A mesh should have a material");

        sRenderableMesh renderableMesh = { meshInstance.get(), color, transform };
        if (material->_transparent)
        {
            CHECK_QUEUE_NOT_FULL(_uiTransparentMeshsNb);
            _uiTransparentMeshs[_uiTransparentMeshsNb] = renderableMesh;
            ++_uiTransparentMeshsNb;
        }
        else
        {
            CHECK_QUEUE_NOT_FULL(_uiOpaqueMeshsNb);
            _uiOpaqueMeshs[_uiOpaqueMeshsNb] = renderableMesh;
            ++_uiOpaqueMeshsNb;
        }
    }
}

void    RenderQueue::clear()
{
    std::memset(_opaqueMeshs.data(), 0, _opaqueMeshsNb * sizeof(void*));
    _opaqueMeshsNb = 0;

    std::memset(_transparentMeshs.data(), 0, _transparentMeshsNb * sizeof(void*));
    _transparentMeshsNb = 0;

    std::memset(_uiOpaqueMeshs.data(), 0, _uiOpaqueMeshsNb * sizeof(void*));
    _uiOpaqueMeshsNb = 0;

    std::memset(_uiTransparentMeshs.data(), 0, _uiTransparentMeshsNb * sizeof(void*));
    _uiTransparentMeshsNb = 0;
}

std::vector<sRenderableMesh>& RenderQueue::getOpaqueMeshs()
{
    return (_opaqueMeshs);
}

uint32_t    RenderQueue::getOpaqueMeshsNb() const
{
    return (_opaqueMeshsNb);
}

std::vector<sRenderableMesh>& RenderQueue::getTransparentMeshs()
{
    return (_transparentMeshs);
}

uint32_t    RenderQueue::getTransparentMeshsNb() const
{
    return (_transparentMeshsNb);
}

std::vector<sRenderableMesh>& RenderQueue::getUIOpaqueMeshs()
{
    return (_uiOpaqueMeshs);
}

uint32_t    RenderQueue::getUIOpaqueMeshsNb() const
{
    return (_uiOpaqueMeshsNb);
}

std::vector<sRenderableMesh>& RenderQueue::getUITransparentMeshs()
{
    return (_uiTransparentMeshs);
}

uint32_t    RenderQueue::getUITransparentMeshsNb() const
{
    return (_uiTransparentMeshsNb);
}
