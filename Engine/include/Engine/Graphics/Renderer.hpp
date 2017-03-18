/**
* @Author   Guillaume Labey
*/

#pragma once

#include <memory>
#include <unordered_map>
#include <glm/mat4x4.hpp>

#include <Engine/Graphics/Camera.hpp>
#include <Engine/Graphics/Framebuffer.hpp>
#include <Engine/Graphics/Light.hpp>
#include <Engine/Graphics/Material.hpp>
#include <Engine/Graphics/ModelInstance.hpp>
#include <Engine/Graphics/RenderQueue.hpp>
#include <Engine/Graphics/ShaderProgram.hpp>
#include <Engine/Graphics/Texture.hpp>

class Renderer
{
public:
    Renderer();
    ~Renderer();

    static std::shared_ptr<Renderer>    getInstance();

    bool                                initialize();
    void                                onWindowResize();

    Camera*                             getCurrentCamera();
    void                                setCurrentCamera(Camera* camera);

    void                                beginFrame();
    void                                endFrame();
    void                                render(Camera* camera, RenderQueue& renderQueue);

private:
    void                                sceneRenderPass(Camera* camera, RenderQueue& renderQueue);
    void                                finalBlendingPass();
    void                                renderOpaqueObjects(std::vector<sRenderableMesh>& meshs,
                                                            uint32_t meshsNb,
                                                            std::vector<Light*>& lights,
                                                            uint32_t lightsNb);
    void                                renderTransparentObjects(std::vector<sRenderableMesh>& meshs,
                                                                uint32_t meshsNb,
                                                                std::vector<Light*>& lights,
                                                                uint32_t lightsNb);

    bool                                setupFrameBuffer();
    void                                setupShaderPrograms();

private:
    // One ShaderProgram for each possible permutation
    std::unordered_map<int, ShaderProgram> _shaderPrograms;
    ShaderProgram*                      _currentShaderProgram{nullptr};

    ShaderProgram                       _finalBlendingShaderProgram;
    // Buffer containing the plane vertices used for final blending
    // Should fit the size of the screen
    Buffer                              _finalBlendingPlane;


    // Singleton instance
    static std::shared_ptr<Renderer>    _instance;

    Camera*                             _currentCamera;
    Camera                              _UICamera;

    // Used when there is no light
    Light                               _defaultLight;

    // Used for UI
    Light                               _UILight;

    Framebuffer                         _frameBuffer;
    std::unique_ptr<Texture>            _colorAttachment;
    std::unique_ptr<Texture>            _depthAttachment;
};
