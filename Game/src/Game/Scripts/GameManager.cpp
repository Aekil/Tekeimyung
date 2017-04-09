#include <Engine/EntityFactory.hpp>
#include <Game/Scripts/GameManager.hpp>

#define LAYER_NUMBER 10

void GameManager::start()
{
    for (int x = 0; x < this->mapSizeX; x++)
    {
        for (int z = 0; z < this->mapSizeZ; z++)
        {
            if ((int)(this->firstLayerPattern[x][z] / 10) == 0)
            {
                if ((int)(this->firstLayerPattern[x][z] % 10) == 1)
                    this->firstLayerEntities[x][z] = this->Instantiate("TILE_FLOOR", glm::vec3(x * 25, 0, z * 25));
                else if ((int)(this->firstLayerPattern[x][z] % 10) == 2)
                    this->firstLayerEntities[x][z] = this->Instantiate("SPAWNER", glm::vec3(x * 25, 0, z * 25));
            }
            else
            {
                Entity* entity;

                if ((int)(this->firstLayerPattern[x][z] % 10) == 1)
                {
                    entity = this->Instantiate("TILE_FLOOR", glm::vec3(x * 25, 0, z * 25));
                }
                else if ((int)(this->firstLayerPattern[x][z] % 10) == 2)
                {
                    entity = this->Instantiate("SPAWNER", glm::vec3(x * 25, 0, z * 25));

                    entity->getComponent<sScriptComponent>()->enabled = false;
                }

                entity->getComponent<sRenderComponent>()->enabled = false;

                this->firstLayerEntities[x][z] = entity;
                this->_mapParts[(int)(this->firstLayerPattern[x][z] / 10)].push_back(entity);
            }
        }
    }

    for (auto& layerPath : layerPaths)
    {
        std::vector<glm::vec3> path;
        for (auto& tileIndex : layerPath)
        {
            Entity* tile = this->firstLayerEntities[tileIndex.y][tileIndex.x];
            sTransformComponent* tileTransform = tile->getComponent<sTransformComponent>();
            path.push_back(tileTransform->getPos());
        }
        this->paths.push_back(path);
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