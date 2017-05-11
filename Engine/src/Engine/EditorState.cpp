/**
* @Author   Guillaume Labey
*/

#include <Engine/EntityFactory.hpp>
#include <Engine/EditorState.hpp>
#include <Engine/Graphics/Renderer.hpp>
#include <Engine/Debug/LevelEntitiesDebugWindow.hpp>
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
    _world.addSystem<RenderingSystem>(_world.getSystem<ParticleSystem>()->getEmitters());

    _world.getSystem<RenderingSystem>()->attachCamera(&_camera);
}

bool    EditorState::init()
{
    SoundManager::getInstance()->setVolumeAllChannels(0.3f);
    initCamera();

    return (true);
}

bool    EditorState::update(float elapsedTime)
{
    Camera* selectedCamera{nullptr};
    updateCamera(elapsedTime);
    handleObjectSelection();

    if (!_cameraPreview)
    {
        _cameraPreview = EntityFactory::createEntity("CAMERA_PREVIEW", false);
        _cameraPreviewTransform = _cameraPreview->getComponent<sTransformComponent>();
        _cameraPreviewRender = _cameraPreview->getComponent<sRenderComponent>();
        _cameraPreviewUI = _cameraPreview->getComponent<sUiComponent>();
    }

    _cameraPreviewRender->display = false;

    // Get selected camera
    {
        Entity::sHandle selectedEntityHandler = LevelEntitiesDebugWindow::getSelectedEntityHandler();
        Entity* selectedEntity = _world.getEntityManager()->getEntity(selectedEntityHandler);
        if (selectedEntity)
        {
            sCameraComponent* cameraComponent = selectedEntity->getComponent<sCameraComponent>();
            if (cameraComponent)
            {
                selectedCamera = &cameraComponent->camera;
                _cameraPreviewRender->display = true;
            }
        }
    }


    // Update the GameState before because we want to draw the camera preview after drawing editor scene
    if (!GameState::update(elapsedTime))
    {
        return (false);
    }

    _cameraPreviewRender->display = false;

    if (selectedCamera)
    {
        renderCameraPreview(*selectedCamera);
        return (true);
    }

    _cameraPreviewRender->display = false;

    return (true);
}

void    EditorState::initCamera()
{
    _camera.translate(glm::vec3(550.0f, 450.0f, 700.0f));
    _camera.rotate(-35.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    _camera.rotate(47.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    _camera.setFar(5000.0f);

    // Set camera viewport
    Camera::sViewport viewportRect;
    viewportRect.offset.x = 0.0f;
    viewportRect.offset.y = 0.0f;
    viewportRect.extent.width = 1.0f;
    viewportRect.extent.height = 1.0f;
    _camera.setProjType(Camera::eProj::PERSPECTIVE);
    _camera.setViewportRect(viewportRect);
}

void    EditorState::updateCamera(float elapsedTime)
{
    auto &gameWindow = GameWindow::getInstance();
    auto &keyboard = gameWindow->getKeyboard();
    auto &mouse = gameWindow->getMouse();

    // Move the camera along forward vector with mouse scroll
    {
        auto &&scroll = mouse.getScroll();
        static double lastScrollOffset;
        static float scrollSpeed = 3000.0f;


        double offset = scroll.yOffset - lastScrollOffset;

        if (offset)
            _camera.translate(glm::vec3(0.0f, 0.0f, (float)(offset * elapsedTime * scrollSpeed)), Transform::eTransform::LOCAL);
        lastScrollOffset = scroll.yOffset;
    }

    // Get mouse position (is used for camera translation and rotation)
    auto& cursor = mouse.getCursor();
    glm::vec2 mousePos{cursor.getX(), cursor.getY()};
    static glm::vec2 lastPosition = mousePos;

    // Update position
    {
        static float movementSpeed = 30.0f;

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

            _camera.rotate(-mouseMovement.x, {0.0f, 1.0f, 0.0f});
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
    // or the mouse if over the guizmos
    if (keyboard.getStateMap()[Keyboard::eKey::LEFT_ALT] == Keyboard::eKeyState::KEY_PRESSED ||
        keyboard.getStateMap()[Keyboard::eKey::LEFT_ALT] == Keyboard::eKeyState::KEY_MAINTAINED ||
        ImGuizmo::IsOver())
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
            LevelEntitiesDebugWindow::setSelectedEntityHandler(selectedEntity->handle);
        }
        else
        {
            LevelEntitiesDebugWindow::setSelectedEntityHandler(0);
        }
    }
}

void    EditorState::renderCameraPreview(Camera& camera)
{
    auto renderSystem = _world.getSystem<RenderingSystem>();


    // Save viewport because we'll modify it
    Camera::sViewport viewportRect = camera.getViewportRect();
    // Modify viewport for preview
    camera.setViewportRect({
        0.3f * viewportRect.offset.x, // offset.x
        0.3f * viewportRect.offset.y, // offset.y
        0.3f * viewportRect.extent.width, // extent.width
        0.3f * viewportRect.extent.height // extent.height
    });

    // Attach preview camera
    renderSystem->attachCamera(&camera);

    // Update camera preview background
    {
        glm::vec3 scale;
        scale.x = (0.3f * _camera.getViewport().extent.width) / SIZE_UNIT;
        scale.y = (0.3f * _camera.getViewport().extent.height) / SIZE_UNIT;
        scale.z = 1.0f;

        _cameraPreviewTransform->setScale(scale);
        _cameraPreviewUI->needUpdate = true;
    }


    // Set scissor to the size of the camera viewport
    // because we don't want the camera preview to be rendered outside the window preview
    glScissor(0,
            0,
            static_cast<uint32_t>(0.3f * GameWindow::getInstance()->getBufferWidth()),
            static_cast<uint32_t>(0.3f * GameWindow::getInstance()->getBufferHeight()));

    // Render camera preview
    renderSystem->update(*_world.getEntityManager(), 0.0f);

    // Reset scissor
    glScissor(0,
            0,
            (uint32_t)GameWindow::getInstance()->getBufferWidth(),
            (uint32_t)GameWindow::getInstance()->getBufferHeight());

    // Reset viewport
    camera.setViewportRect(viewportRect);

    // Attach editor camera
    renderSystem->attachCamera(&_camera);
    Renderer::getInstance()->setCurrentCamera(&_camera);
}
