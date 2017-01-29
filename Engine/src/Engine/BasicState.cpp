/**
* @Author   Guillaume Labey
*/

#include <Engine/Systems/ButtonSystem.hpp>
#include <Engine/Systems/CollisionSystem.hpp>
#include <Engine/Systems/GravitySystem.hpp>
#include <Engine/Systems/ParticleSystem.hpp>
#include <Engine/Systems/RenderingSystem.hpp>
#include <Engine/Systems/ResolutionSystem.hpp>
#include <Engine/Systems/ScriptSystem.hpp>
#include <Engine/Systems/UISystem.hpp>
#include <Engine/Systems/MouseSystem.hpp>

#include <Engine/BasicState.hpp>

BasicState::~BasicState() {}

void    BasicState::setupSystems()
{
    _world.addSystem<ScriptSystem>();
    _world.addSystem<MouseSystem>();
    _world.addSystem<CollisionSystem>();
    _world.addSystem<ResolutionSystem>();
    _world.addSystem<ParticleSystem>();
    _world.addSystem<UISystem>();
    _world.addSystem<ButtonSystem>(_gameStateManager);
    _world.addSystem<RenderingSystem>(nullptr, _world.getSystem<ParticleSystem>()->getEmitters());
}
