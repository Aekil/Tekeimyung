/**
* @Author   Guillaume Labey
*/

#pragma once


#include <Engine/Graphics/ShaderProgram.hpp>
#include <Engine/Graphics/ModelInstance.hpp>
#include <Engine/Graphics/Camera.hpp>
#include <Engine/Graphics/RenderQueue.hpp>

#include <glm/mat4x4.hpp>

class Renderer
{
public:
    Renderer();
    ~Renderer();

    static std::shared_ptr<Renderer>    getInstance();

    bool                                initialize();
    void                                onWindowResize();

    void                                render(Camera* camera, RenderQueue& renderQueue);
    void                                renderOpaqueObjects(Camera* camera, std::vector<sRenderableMesh>& meshs, uint32_t meshsNb);
    void                                renderTransparentObjects(Camera* camera, std::vector<sRenderableMesh>& meshs, uint32_t meshsNb);

    ShaderProgram&                      getShaderProgram();
    Camera*                             getCurrentCamera();
    void                                setCurrentCamera(Camera* camera);

private:
    ShaderProgram                       _shaderProgram;


    // Singleton instance
    static std::shared_ptr<Renderer>    _instance;

    Camera*                             _currentCamera;
    Camera                              _UICamera;
};
