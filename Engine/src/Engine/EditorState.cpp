/**
* @Author   Guillaume Labey
*/

#include <Engine/EntityFactory.hpp>
#include <Engine/EditorState.hpp>
#include <Engine/LevelEntitiesDebugWindow.hpp>
#include <Engine/Physics/Physics.hpp>
#include <Engine/Sound/SoundManager.hpp>
#include <Engine/Systems/RenderingSystem.hpp>
#include <Engine/Systems/UISystem.hpp>
#include <Engine/Window/GameWindow.hpp>


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
    updateCamera(elapsedTime);
    handleObjectSelection();
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
    _camera.setProjType(Camera::eProj::PERSPECTIVE);
    _camera.setZoom(0.5f);
}

void    EditorState::updateCamera(float elapsedTime)
{
    auto &gameWindow = GameWindow::getInstance();
    auto &keyboard = gameWindow->getKeyboard();
    auto &mouse = gameWindow->getMouse();

    // Update zoom
    {
        auto &&scroll = mouse.getScroll();
        static double lastScrollOffset;
        static float scrollSpeed = 5.0f;


        double offset = scroll.yOffset - lastScrollOffset;

        if (offset)
            _camera.zoom((float)(-offset * elapsedTime * scrollSpeed));
        lastScrollOffset = scroll.yOffset;
    }

    // Get mouse position (is used for camera translation and rotation)
    auto& cursor = mouse.getCursor();
    glm::vec2 mousePos{cursor.getX(), cursor.getY()};
    static glm::vec2 lastPosition = mousePos;

    // Update position
    // TODO: use elapsedTime
    {
        static float movementSpeed = 20.0f;

        if (mouse.getStateMap()[Mouse::eButton::MOUSE_BUTTON_3] == Mouse::eButtonState::CLICK_MAINTAINED)
        {

            glm::vec2 mouseMovement = mousePos - lastPosition;
            mouseMovement *= elapsedTime * movementSpeed;

            _camera.translate({-mouseMovement.x, mouseMovement.y, 0.0f}, Camera::eTransform::LOCAL);
        }
    }

    // Update rotation
    {
        static float rotationSpeed = 20.0f;

        if (keyboard.getStateMap()[Keyboard::eKey::LEFT_ALT] == Keyboard::eKeyState::KEY_MAINTAINED &&
            mouse.getStateMap()[Mouse::eButton::MOUSE_BUTTON_1] == Mouse::eButtonState::CLICK_MAINTAINED)
        {

            glm::vec2 mouseMovement = mousePos - lastPosition;
            mouseMovement *= elapsedTime * rotationSpeed;

            _camera.rotate(mouseMovement.x, {0.0f, 1.0f, 0.0f});
            _camera.rotate(-mouseMovement.y, {1.0f, 0.0f, 0.0f});
        }
    }

    lastPosition = mousePos;
}

void    EditorState::handleObjectSelection()
{
    auto &gameWindow = GameWindow::getInstance();
    auto &keyboard = gameWindow->getKeyboard();
    auto &mouse = gameWindow->getMouse();

    // The camera is rotating in EditorState::updateCamera
    if (keyboard.getStateMap()[Keyboard::eKey::LEFT_ALT] == Keyboard::eKeyState::KEY_PRESSED ||
        keyboard.getStateMap()[Keyboard::eKey::LEFT_ALT] == Keyboard::eKeyState::KEY_MAINTAINED)
    {
        return;
    }

    if (mouse.getStateMap()[Mouse::eButton::MOUSE_BUTTON_1] == Mouse::eButtonState::CLICK_PRESSED)
    {
        Entity* selectedEntity;
        auto& cursor = mouse.getCursor();

        Ray ray = _camera.screenPosToRay((float)cursor.getX(), (float)cursor.getY());
        Physics::raycast(ray, &selectedEntity);

        if (selectedEntity)
        {
            LevelEntitiesDebugWindow::setSelectedEntityId(selectedEntity->id);
        }
    }
}
