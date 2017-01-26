/**
* @Author   Guillaume Labey
*/

#include <iostream>

#include <Engine/Utils/Exception.hpp>

#include <Engine/Graphics/Renderer.hpp>

std::shared_ptr<Renderer>   Renderer::_instance = nullptr;

Renderer::Renderer(): _currentCamera(nullptr) {}

Renderer::~Renderer() {}

std::shared_ptr<Renderer>   Renderer::getInstance()
{
    if (!_instance)
        _instance = std::make_shared<Renderer>();

    return (_instance);
}

bool    Renderer::initialize()
{
    try
    {
        _shaderProgram.attachShader(GL_VERTEX_SHADER, "resources/shaders/shader.vert");
        _shaderProgram.attachShader(GL_FRAGMENT_SHADER, "resources/shaders/shader.frag");
        _shaderProgram.link();
        _shaderProgram.use();

        // Set texture location unit
        // Must be the same unit as material textures. See Material::loadFromAssimp
        glUniform1i(_shaderProgram.getUniformLocation("AmbientTexture"), 0);
        glUniform1i(_shaderProgram.getUniformLocation("DiffuseTexture"), 1);

        _cameraUbo.setBindingPoint(1);
        _cameraUbo.bind(_shaderProgram, "camera");
    }
    catch(const Exception& e)
    {
        std::cerr << e.what() << std::endl;
        return (false);
    }


    // Enable blend for transparency
/*    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
*/
    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Activate back culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    return (true);
}

void    Renderer::render(Camera* camera, std::shared_ptr<Model> model,
                                        const glm::vec4& modelColor, const glm::mat4& modelTransform)
{
    camera->updateUboData(_cameraUbo, _currentCamera != camera);
    model->draw(_shaderProgram, modelColor, modelTransform);

    _currentCamera = camera;
}

ShaderProgram&  Renderer::getShaderProgram()
{
    return (_shaderProgram);
}

Camera* Renderer::getCurrentCamera()
{
    return (_currentCamera);
}

void    Renderer::setCurrentCamera(Camera* camera)
{
    _currentCamera = camera;
}
