/**
* @Author   Guillaume Labey
*/

#include <imgui.h>
#include <imgui_impl_glfw_gl3.h>
#include <glm/glm.hpp>

#include <Engine/Utils/Exception.hpp>
#include <Engine/Window/Keyboard.hpp>
#include <Engine/Window/HandleFullscreenEvent.hpp>
#include <Engine/Window/GameWindow.hpp>
#include <Engine/Sound/SoundManager.hpp>
#include <Engine/Graphics/Camera.hpp>
#include <Engine/Graphics/Animation.hpp>
#include <Engine/Physics/Collisions.hpp>

#include <Engine/Systems/RenderingSystem.hpp>
#include <Engine/Systems/RigidBodySystem.hpp>
#include <Engine/Systems/CollisionSystem.hpp>
#include <Engine/Systems/ParticleSystem.hpp>
#include <Engine/Systems/ScriptSystem.hpp>
#include <Engine/Systems/UISystem.hpp>
#include <Engine/Systems/MouseSystem.hpp>
#include <Engine/Components.hh>
#include <Engine/EntityFactory.hpp>
#include <Game/GameStates/PauseState.hpp>

#include <Game/GameStates/PlayState.hpp>


PlayState::~PlayState() {}

void    PlayState::onEnter() {}

void    PlayState::setupSystems()
{
    _world.addSystem<ScriptSystem>();
    _world.addSystem<MouseSystem>();
    _world.addSystem<RigidBodySystem>();
    _world.addSystem<CollisionSystem>();
    _world.addSystem<ParticleSystem>();
    _world.addSystem<UISystem>();
    _world.addSystem<RenderingSystem>(&_camera, _world.getSystem<ParticleSystem>()->getEmitters());
}

bool    PlayState::init()
{
    EntityManager* em = _world.getEntityManager();

    _map = new Map(*em, 20, 15, 4);

    initCamera();
    initEntities();

    _pair = std::make_pair(Keyboard::eKey::F, new HandleFullscreenEvent());

    _backgroundMusic = EventSound::getEventByEventType(eEventSound::BACKGROUND);

    return (true);
}

bool    PlayState::update(float elapsedTime)
{
    auto& gameWindow = GameWindow::getInstance();
    auto &&keyboard = GameWindow::getInstance()->getKeyboard();
    auto &&mouse = GameWindow::getInstance()->getMouse();

    if (keyboard.getStateMap()[_pair.first] == Keyboard::eKeyState::KEY_PRESSED)
        _pair.second->execute();

    if (keyboard.getStateMap()[Keyboard::eKey::ESCAPE] == Keyboard::eKeyState::KEY_PRESSED ||
        gameWindow->hasLostFocus())
    {
        _gameStateManager->addState<PauseState>();
    }

    updateCameraInputs(elapsedTime);

    // Play background music
    #if (ENABLE_SOUND)
        if (_backgroundMusic->soundID != -1 && !SoundManager::getInstance()->isSoundPlaying(_backgroundMusic->soundID))
        {
            SoundManager::getInstance()->playSound(_backgroundMusic->soundID);
        }
    #endif

    return (GameState::update(elapsedTime));
}

void    PlayState::initCamera()
{
    _camera.translate(glm::vec3(650.0f, 450.0f, 600.0f));
    _camera.setDir(glm::vec3(-30.0f));

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

void    PlayState::initEntities()
{
    EntityManager* em = _world.getEntityManager();

    // Play first animation of entities
    for (auto entity: em->getEntities())
    {
        sRenderComponent* render = entity.second->getComponent<sRenderComponent>();
        if (render && render->_animator.getAnimationsNb() > 0)
        {
            AnimationPtr currentAnimation = render->_animator.getAnimations()[0];
            render->_animator.play(currentAnimation->getName());
        }
    }
}

void    PlayState::updateCameraInputs(float elapsedTime)
{
    auto &gameWindow = GameWindow::getInstance();
    auto &mouse = gameWindow->getMouse();
    auto &keyboard = gameWindow->getKeyboard();

    // update Projection type
    {
        if (keyboard.isPressed(Keyboard::eKey::O))
            _camera.setProjType(Camera::eProj::ORTHOGRAPHIC_3D);
        else if (keyboard.isPressed(Keyboard::eKey::P))
            _camera.setProjType(Camera::eProj::PERSPECTIVE);
    }

    // update zoom
    {
        auto &&scroll = mouse.getScroll();
        static double lastScrollOffset;


        double offset = scroll.yOffset - lastScrollOffset;

        if (offset)
            _camera.zoom((float)(-offset * elapsedTime));
        lastScrollOffset = scroll.yOffset;
    }

    // update camera position when reaching edge
    {
        static float edgeDist = 80.0f;
        static float moveSpeed = 5.0f;
        auto& cursor = mouse.getCursor();
        ImGuiIO& io = ImGui::GetIO();

        if (!io.WantCaptureMouse)
        {
            if (cursor.getX() > gameWindow->getBufferWidth() - edgeDist)
            {
                _camera.translate({moveSpeed, 0.0f, -moveSpeed});
            }
            if (cursor.getX() < edgeDist)
            {
                _camera.translate({-moveSpeed, 0.0f, moveSpeed});
            }
            if (cursor.getY() > gameWindow->getBufferHeight() - edgeDist)
            {
                _camera.translate({moveSpeed, 0.0f, moveSpeed});
            }
            if (cursor.getY() < edgeDist)
            {
                _camera.translate({-moveSpeed, 0.0f, -moveSpeed});
            }
        }

    }
}
