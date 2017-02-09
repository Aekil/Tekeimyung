/**
* @Author   Guillaume Labey
*/

#include <Engine/Systems/ButtonSystem.hpp>
#include <Engine/Systems/CollisionSystem.hpp>
#include <Engine/Systems/ParticleSystem.hpp>
#include <Engine/Systems/RenderingSystem.hpp>
#include <Engine/Systems/ResolutionSystem.hpp>
#include <Engine/Systems/RigidBodySystem.hpp>
#include <Engine/Systems/ScriptSystem.hpp>
#include <Engine/Systems/UISystem.hpp>
#include <Engine/Systems/MouseSystem.hpp>

#include <Engine/BasicState.hpp>

BasicState::~BasicState() {}

void    BasicState::setupSystems()
{
    _world.addSystem<ScriptSystem>();
    _world.addSystem<MouseSystem>();
    _world.addSystem<RigidBodySystem>();
    _world.addSystem<CollisionSystem>();
    _world.addSystem<ResolutionSystem>();
    _world.addSystem<ParticleSystem>();
    _world.addSystem<UISystem>();
    _world.addSystem<ButtonSystem>(_gameStateManager);
    _world.addSystem<RenderingSystem>(&_camera, _world.getSystem<ParticleSystem>()->getEmitters());
}

bool    BasicState::init()
{
    initCamera();
    return (true);
}

void    BasicState::initCamera()
{
    _camera.translate(glm::vec3(350.0f, 250.0f, 300.0f));
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
