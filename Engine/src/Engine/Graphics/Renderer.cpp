/**
* @Author   Guillaume Labey
*/

#include <iostream>

#include <Engine/Utils/Exception.hpp>
#include <Engine/Window/GameWindow.hpp>

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

    onWindowResize();

    // Enable blend for transparency
/*    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
*/
    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Activate back culling
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);
    return (true);
}

void    Renderer::onWindowResize()
{
    auto gameWindow = GameWindow::getInstance();

    // Set camera screen
    Camera::sScreen screen;
    screen.right = (float)gameWindow->getBufferWidth();
    screen.left = 0;
    screen.top = (float)gameWindow->getBufferHeight();
    screen.bottom = 0;
    _UICamera.setScreen(screen);
    _UICamera.setProjType(Camera::eProj::ORTHOGRAPHIC_2D);
    _UICamera.update(_shaderProgram, 0);
    _UICamera.updateUboData(_cameraUbo, true);
}

void    Renderer::render(Camera* camera, ModelInstance* model,
                                        const glm::vec4& modelColor, const glm::mat4& modelTransform)
{
    camera->updateUboData(_cameraUbo, _lastCamera != camera);
    model->draw(_shaderProgram, modelColor, modelTransform);

    _currentCamera = camera;
    _lastCamera = camera;
}

// TODO: draw all UI after models
// Curently the UI is drawn at the same time as the transparent entities if the model is a plane
void    Renderer::renderUI(ModelInstance* model, const glm::vec4& modelColor, const glm::mat4& modelTransform)
{
    if (_lastCamera != &_UICamera)
        _UICamera.updateUboData(_cameraUbo, true);
    model->draw(_shaderProgram, modelColor, modelTransform);
    _lastCamera = &_UICamera;
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
