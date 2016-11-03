#include <Systems/TowerAISystem.hpp>
#include <Core/Components.hh>

#include <Utils/Logger.hpp>
#include <glm/gtx/string_cast.hpp>

TowerAISystem::TowerAISystem(Map* map) : _map(map)
{
    //this->addDependency<sTowerAIComponent>();
    //this->addDependency<sPositionComponent>();
}

TowerAISystem::~TowerAISystem() {}

void    TowerAISystem::update(EntityManager &em, float elapsedTime)
{
    //forEachEntity(em, [&](Entity *entity) {
    //    sPositionComponent  *position = entity->getComponent<sPositionComponent>();

    //    //LOG_INFO("Position: ", glm::to_string(position->value));
    //});
    forEachEntity(em, [&](Entity *entity) {

    });
}