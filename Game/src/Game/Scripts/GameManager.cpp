#include <Engine/EntityFactory.hpp>
#include <Game/Scripts/GameManager.hpp>

void GameManager::start()
{
    for (int x = 0; x < this->mapSizeX; x++)
    {
        for (int z = 0; z < this->mapSizeZ; z++)
        {
            if (this->firstLayerPattern[x][z] == 1)
                this->firstLayerEntities[x][z] = this->Instantiate("BLOCK_BROWN", glm::vec3(x * 25, 0, z * 25));
            else if (this->firstLayerPattern[x][z] == 2)
                this->firstLayerEntities[x][z] = this->Instantiate("SPAWNER", glm::vec3(x * 25, 0, z * 25));
            else if (this->firstLayerPattern[x][z] > 3)
            {
                this->firstLayerEntities[x][z] == this->Instantiate("INVISIBLE_BLOCK", glm::vec3(x * 25, 0, z * 25));
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
