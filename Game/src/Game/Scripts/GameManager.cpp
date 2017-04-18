#include <Engine/EntityFactory.hpp>
#include <Game/Scripts/GameManager.hpp>

GameManager::GameManager()
{
    std::memset(spawnersPaths, 0, sizeof(spawnersPaths[0][0]) * mapSizeX * mapSizeZ);
    std::memset(firstLayerEntities, 0, sizeof(firstLayerEntities[0][0]) * mapSizeX * mapSizeZ);
}

void GameManager::start()
{
    for (int x = 0; x < this->mapSizeX; x++)
    {
        for (int z = 0; z < this->mapSizeZ; z++)
        {
            if ((int)(this->firstLayerPattern[x][z] / LAYER_NUMBER) == 0)
            {
                if ((int)(this->firstLayerPattern[x][z] % LAYER_NUMBER) == 1)
                    this->firstLayerEntities[x][z] = this->Instantiate("TILE_FLOOR", glm::vec3(x * 25, 0, z * 25));
                else if ((int)(this->firstLayerPattern[x][z] % LAYER_NUMBER) == 2)
                    this->firstLayerEntities[x][z] = this->Instantiate("SPAWNER", glm::vec3(x * 25, 0, z * 25));
                else if ((int)(this->firstLayerPattern[x][z] % LAYER_NUMBER) == 3)
                    this->firstLayerEntities[x][z] = this->Instantiate("TILE_BASE_TURRET", glm::vec3(x * 25, 0, z * 25));
            }
            else
            {
                Entity* entity;

                if ((int)(this->firstLayerPattern[x][z] % LAYER_NUMBER) == 1)
                {
                    entity = this->Instantiate("TILE_FLOOR", glm::vec3(x * 25, 0, z * 25));
                }
                else if ((int)(this->firstLayerPattern[x][z] % LAYER_NUMBER) == 2)
                {
                    entity = this->Instantiate("SPAWNER", glm::vec3(x * 25, 0, z * 25));

                    entity->getComponent<sScriptComponent>()->enabled = false;
                }

                entity->getComponent<sRenderComponent>()->enabled = false;

                this->firstLayerEntities[x][z] = entity;
                this->_mapParts[(int)(this->firstLayerPattern[x][z] / LAYER_NUMBER)].push_back(entity);
            }
        }
    }
}

void GameManager::update(float dt)
{
    EntityManager* em = EntityFactory::getBindedEntityManager();
    const auto& projectiles = em->getEntitiesByTag("Projectile");
    for (auto &projectile : projectiles)
    {
        sTransformComponent* transform = projectile->getComponent<sTransformComponent>();
        if (transform->getPos().x < 0 || transform->getPos().z < 0 ||
            transform->getPos().x > this->mapSizeX * 25 ||
            transform->getPos().z > this->mapSizeZ * 25)
        {
            em->destroyEntityRegister(projectile);
        }
    }
}

void GameManager::displayMapParts(int part)
{
    auto mapParts = this->_mapParts[part];

    for (auto& mapPart : mapParts)
    {
        auto render = mapPart->getComponent<sRenderComponent>();
        render->enabled = true;
        auto script = mapPart->getComponent<sScriptComponent>();
        script->enabled = true;
    }
}
