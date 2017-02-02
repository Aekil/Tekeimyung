#include <Game/Scripts/GameManager.hpp>

void GameManager::Start()
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

void GameManager::Update(float dt)
{

}