/**
* @Author   Guillaume Labey
*/

#pragma once

#include <array>
#include <memory>
#include <unordered_map>
#include <glm/mat4x4.hpp>

#include <ECS/Entity.hpp>
#include <Engine/Core/Components/RenderComponent.hh>
#include <Engine/Graphics/Camera.hpp>
#include <Engine/Graphics/Framebuffer.hpp>
#include <Engine/Graphics/Light.hpp>
#include <Engine/Graphics/Material.hpp>
#include <Engine/Graphics/ModelInstance.hpp>
#include <Engine/Graphics/RenderQueue.hpp>
#include <Engine/Graphics/ShaderProgram.hpp>
#include <Engine/Graphics/UniformBuffer.hpp>
#include <Engine/Graphics/Texture.hpp>

class Model2DRenderer
{
public:
    Model2DRenderer() = default;
    ~Model2DRenderer() = default;

    static std::shared_ptr<Model2DRenderer>     getInstance();


    std::unique_ptr<Texture>                    generateTextureFromModel(sRenderComponent* renderComponent,
                                                                        uint32_t width,
                                                                        uint32_t height);
    std::unique_ptr<Texture>                    generateTextureFromModel(Entity* modelEntity,
                                                                        uint32_t width,
                                                                        uint32_t height);
    bool                                        renderModelOnPlane(const std::string& modelEntityName,
                                                                    const std::string& planeEntityTagName,
                                                                    uint32_t width,
                                                                    uint32_t height);
    bool                                        renderModelOnPlane(Entity* modelEntity,
                                                                    const std::string& planeEntityTagName,
                                                                    uint32_t width,
                                                                    uint32_t height);
    bool                                        renderModelOnPlane(const std::string& modelEntityName,
                                                                    Entity* planeEntity,
                                                                    uint32_t width,
                                                                    uint32_t height);
private:
    void                                        initialize();

private:
    // Singleton instance
    static std::shared_ptr<Model2DRenderer>    _instance;


    Framebuffer                                 _2DFramebuffer;
    std::vector<std::array<Framebuffer, 2>>     _2DBlurFramebuffers;
    UniformBuffer                               _2DRenderBuffer;
    RenderQueue                                 _2DRenderQueue;
    Camera                                      _2DRenderCamera;
    Light                                       _2DRenderLight;

    // Store the textures rendered in Model2DRenderer::renderModelOnPlane
    // We need to do this because we give the pointer to the plane material
    std::vector<std::unique_ptr<Texture>>       _renderedTextures;
};
