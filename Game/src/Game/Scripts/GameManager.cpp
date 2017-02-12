#include <Engine/EntityFactory.hpp>
#include <Game/Scripts/GameManager.hpp>

void GameManager::start()
{
    for (int z = 0; z < this->mapSize; z++) {
        for (int x = 0; x < this->mapSize; x++) {
            if (this->firstLayerPattern[x][z] == 1)
                this->Instantiate("SPAWNER", glm::vec3(x * 25, 0, z * 25));
            else
                this->Instantiate("BLOCK_BROWN", glm::vec3(x * 25, 0, z * 25));
        }
    }

    for (int x = 0; x < this->mapSize; x++) {
        for (int z = 0; z < this->mapSize; z++) {
            if (this->secondLayerPattern[x][z] == 1) {
                this->Instantiate("BLOCK_GREEN", glm::vec3(x * 25, 12.5, z * 25));
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
        if (transform->pos.x < 0 || transform->pos.z < 0 ||
            transform->pos.x > this->mapSize * 25 ||
            transform->pos.z > this->mapSize * 25)
        {
            em->destroyEntityRegister(projectile);
        }
    }
}
