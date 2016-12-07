#pragma once


#include <Engine/Graphics/ShaderProgram.hpp>
#include <Engine/Graphics/Model.hpp>
#include <Engine/Graphics/Camera.hpp>

#include <glm/mat4x4.hpp>

class Renderer
{
public:
    Renderer();
    ~Renderer();

    static std::shared_ptr<Renderer>    getInstance();

    bool                                initialize();

    void                                render(Camera* camera, std::shared_ptr<Model> model,
                                            const glm::vec4& modelColor, const glm::mat4& modelTransform);

    ShaderProgram&                      getShaderProgram();

private:
    ShaderProgram                       _shaderProgram;


    // Singleton instance
    static std::shared_ptr<Renderer>    _instance;


    UniformBuffer                       _cameraUbo;

    Camera*                             _currentCamera;
};
