#include <Game/Scripts/GameManager.hpp>

void GameManager::Start()
{
    for (int z = 0; z < this->mapSize; z++) {
        for (int x = 0; x < this->mapSize; x++) {
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
    auto &&mouse = GameWindow::getInstance()->getMouse();

    // Build item if the mouse is pressed 
    if (mouse.getStateMap()[Mouse::eButton::MOUSE_BUTTON_1] == Mouse::eButtonState::CLICK_PRESSED)
    {
        LOG_DEBUG("Coucou je click");
    }
}