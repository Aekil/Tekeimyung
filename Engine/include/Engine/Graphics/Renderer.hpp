/**
* @Author   Guillaume Labey
*/

#pragma once

#include <array>
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
#include <Engine/Graphics/UniformBuffer.hpp>
#include <Engine/Graphics/Texture.hpp>

struct sRenderComponent;

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

    std::unique_ptr<Texture>            generateTextureFromModel(sRenderComponent* renderComponent, uint32_t width, uint32_t height);

private:
    void                                sceneRenderPass(Camera* camera, RenderQueue& renderQueue);
    void                                transparencyPass(Camera* camera, RenderQueue& renderQueue);
    void                                bloomPass();
    void                                finalBlendingPass();
    void                                renderOpaqueObjects(std::vector<sRenderableMesh>& meshs,
                                                            uint32_t meshsNb,
                                                            std::vector<Light*>& lights,
                                                            uint32_t lightsNb);
    void                                renderTransparentObjects(std::vector<sRenderableMesh>& meshs,
                                                                uint32_t meshsNb,
                                                                std::vector<Light*>& lights,
                                                                uint32_t lightsNb);
    void                                renderTexts(std::vector<sRenderableText>& texts,
                                                                uint32_t textsNb);

    bool                                setupFrameBuffer();
    void                                setupShaderPrograms();
    void                                setup2DTextureGeneration();

private:
    void                                initTextRendering();

private:
    // One ShaderProgram for each possible permutation
    std::unordered_map<int, ShaderProgram> _shaderPrograms;
    ShaderProgram*                      _currentShaderProgram{nullptr};

    ShaderProgram                       _textShaderProgram;
    Buffer                              _textPlane;
    UniformBuffer                       _textUBO;

    ShaderProgram                       _finalBlendingShaderProgram;
    ShaderProgram                       _hdrShaderProgram;
    ShaderProgram                       _blurShaderProgram;
    ShaderProgram                       _transparencyShaderProgram;
    // Buffer containing the plane vertices used for final blending and blur
    // Should fit the size of the screen
    Buffer                              _screenPlane;


    // Singleton instance
    static std::shared_ptr<Renderer>    _instance;

    Camera*                             _currentCamera;
    Camera                              _UICamera;

    // Used when there is no light
    Light                               _defaultLight;

    // Used for UI
    Light                               _UILight;

    Framebuffer                         _frameBuffer;
    Framebuffer                         _transparencyFrameBuffer;

    // We have one frame buffer and two color attachments for each blur kernel size
    std::vector<std::array<Framebuffer, 2>>   _blurFramebuffers;


    Framebuffer                         _2DFrameBuffer;
    UniformBuffer                       _2DRenderBuffer;
    RenderQueue                         _2DRenderQueue;
    Camera                              _2DRenderCamera;
    Light                               _2DRenderLight;
};
