/**
* @Author   Julien CHARDON
*/

#include <Engine/EntityFactory.hpp>
#include <Game/Scripts/TutoManager.hpp>

void TutoManager::start()
{
    /*for (int x = 0; x < this->mapSizeX; x++)
    {
        for (int z = 0; z < this->mapSizeZ; z++)
        {
            if ((int)(this->firstLayerPattern[x][z]) == 1)
                this->firstLayerEntities[x][z] = this->Instantiate("TILE_FLOOR", glm::vec3(x * 25, 0, z * 25));
            else if ((int)(this->firstLayerPattern[x][z]) == 2)
                this->firstLayerEntities[x][z] = this->Instantiate("SPAWNER", glm::vec3(x * 25, 0, z * 25));
        }
    }*/
}

void TutoManager::update(float dt)
{
    /*EntityManager* em = EntityFactory::getBindedEntityManager();
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
    }*/
}