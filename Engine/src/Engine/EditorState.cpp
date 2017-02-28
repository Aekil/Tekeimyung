/**
* @Author   Guillaume Labey
*/

#include <Engine/Window/GameWindow.hpp>
#include <Engine/Sound/SoundManager.hpp>

#include <Engine/Systems/RenderingSystem.hpp>
#include <Engine/Systems/UISystem.hpp>
#include <Engine/EntityFactory.hpp>
#include <Engine/EditorState.hpp>


EditorState::~EditorState() {}

void    EditorState::onEnter() {}

void    EditorState::setupSystems()
{
    _world.addSystem<ParticleSystem>(true);
    _world.addSystem<UISystem>();
    _world.addSystem<RenderingSystem>(&_camera, _world.getSystem<ParticleSystem>()->getEmitters());
}

bool    EditorState::init()
{
    SoundManager::getInstance()->setVolume(0.3f);
    initCamera();
    return (true);
}

bool    EditorState::update(float elapsedTime)
{
    auto &gameWindow = GameWindow::getInstance();
    auto &keyboard = gameWindow->getKeyboard();

    // update Projection type
    {
        if (keyboard.isPressed(Keyboard::eKey::O))
            _camera.setProjType(Camera::eProj::ORTHOGRAPHIC_3D);
        else if (keyboard.isPressed(Keyboard::eKey::P))
            _camera.setProjType(Camera::eProj::PERSPECTIVE);
    }

    return (GameState::update(elapsedTime));
}

void    EditorState::initCamera()
{
    _camera.translate(glm::vec3(350.0f, 250.0f, 300.0f));
    _camera.rotate(-35.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    _camera.rotate(-135.0f, glm::vec3(0.0f, 1.0f, 0.0f));

    // Set camera screen
    float size = 500.0f;
    Camera::sScreen screen;
    screen.right = size * _camera.getAspect();
    screen.left = -screen.right;
    screen.top = size;
    screen.bottom = -screen.top;
    _camera.setScreen(screen);
    _camera.setProjType(Camera::eProj::ORTHOGRAPHIC_3D);
    _camera.setZoom(0.5f);
}
