/**
* @Author   Guillaume Labey
*/

#pragma once


#include <Engine/Graphics/ShaderProgram.hpp>
#include <Engine/Graphics/ModelInstance.hpp>
#include <Engine/Graphics/Camera.hpp>

#include <glm/mat4x4.hpp>

class Renderer
{
public:
    Renderer();
    ~Renderer();

    static std::shared_ptr<Renderer>    getInstance();

    bool                                initialize();
    void                                onWindowResize();

    void                                render(Camera* camera, ModelInstance* model,
                                            const glm::vec4& modelColor, const glm::mat4& modelTransform);
    void                                renderUI(ModelInstance* model, const glm::vec4& modelColor, const glm::mat4& modelTransform);

    ShaderProgram&                      getShaderProgram();
    Camera*                             getCurrentCamera();
    void                                setCurrentCamera(Camera* camera);

private:
    ShaderProgram                       _shaderProgram;


    // Singleton instance
    static std::shared_ptr<Renderer>    _instance;


    UniformBuffer                       _cameraUbo;

    Camera*                             _currentCamera;
    Camera*                             _lastCamera;
    Camera                              _UICamera;
};
