/*
** @Author : Simon AMBROISE
*/

#include <Engine/Components.hh>
#include <Engine/EntityFactory.hpp>

#include <Game/Scripts/Enemy.hpp>
#include <Game/Scripts/GameManager.hpp>
#include <Game/Scripts/Spawner.hpp>

void Spawner::start()
{
    auto em = EntityFactory::getBindedEntityManager();
    _transform = this->getComponent<sTransformComponent>();
    auto& pos = _transform->getPos();

    // Init closest path
    {
        Entity* gameManager = em->getEntityByTag("GameManager");
        if (!gameManager)
        {
            LOG_WARN("Can't find entity with GameManager tag");
            return;
        }

        auto scriptComponent = gameManager->getComponent<sScriptComponent>();

        if (!scriptComponent)
        {
            LOG_WARN("Can't find scriptComponent on GameManager entity");
            return;
        }

        _gameManager = scriptComponent->getScript<GameManager>("GameManager");
        _closestPath = getClosestPath();
    }

    // Spawn enemy and set closest path
    {
        Entity* enemy = this->Instantiate("ENEMY", glm::vec3(pos.x, 18.75, pos.z));
        auto scriptComponent = enemy->getComponent<sScriptComponent>();

        if (!scriptComponent)
        {
            LOG_WARN("Can't find scriptComponent on ENEMY entity");
            return;
        }

        Enemy* enemyScript = scriptComponent->getScript<Enemy>("Enemy");

        if (!enemyScript)
        {
            LOG_WARN("Can't find Enemy script on ENEMY entity");
            return;
        }
        enemyScript->setPath(_closestPath);
    }

}

void Spawner::update(float dt)
{

}

std::vector<glm::vec3> Spawner::getClosestPath() const
{
    if (!_gameManager ||
        _gameManager->paths.size() == 0)
    {
        return (std::vector<glm::vec3>());
    }

    float closestDistance = 99999999999999.9f;
    std::vector<glm::vec3> closestPath = _gameManager->paths[0];
    for (auto path: _gameManager->paths)
    {
        if (path.size() == 0)
            continue;
        float distance = glm::distance(_transform->getPos(), path[0]);
        if (distance < closestDistance)
        {
            closestPath = path;
            closestDistance = distance;
        }
    }

    return (closestPath);
}
