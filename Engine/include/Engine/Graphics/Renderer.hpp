/**
* @Author   Guillaume Labey
*/

#pragma once

#include <unordered_map>
#include <glm/mat4x4.hpp>

#include <Engine/Graphics/Camera.hpp>
#include <Engine/Graphics/Light.hpp>
#include <Engine/Graphics/Material.hpp>
#include <Engine/Graphics/ModelInstance.hpp>
#include <Engine/Graphics/RenderQueue.hpp>
#include <Engine/Graphics/ShaderProgram.hpp>

class Renderer
{
public:
    Renderer();
    ~Renderer();

    static std::shared_ptr<Renderer>    getInstance();

    bool                                initialize();
    void                                onWindowResize();

    void                                render(Camera* camera, RenderQueue& renderQueue);
    void                                renderOpaqueObjects(Camera* camera,
                                                            std::vector<sRenderableMesh>& meshs,
                                                            uint32_t meshsNb,
                                                            std::vector<Light*>& lights,
                                                            uint32_t lightsNb);
    void                                renderTransparentObjects(Camera* camera,
                                                                std::vector<sRenderableMesh>& meshs,
                                                                uint32_t meshsNb,
                                                                std::vector<Light*>& lights,
                                                                uint32_t lightsNb);

    Camera*                             getCurrentCamera();
    void                                setCurrentCamera(Camera* camera);

private:
    // One ShaderProgram for each possible permutation
    std::unordered_map<int, ShaderProgram> _shaderPrograms;
    ShaderProgram*                      _currentShaderProgram{nullptr};


    // Singleton instance
    static std::shared_ptr<Renderer>    _instance;

    Camera*                             _currentCamera;
    Camera                              _UICamera;

    // Used when there is no light
    Light                               _defaultLight;

    // Used for UI
    Light                               _UILight;
};
