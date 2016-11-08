#include <cmath>
#include <glm/gtc/matrix_transform.hpp>

#include "Window/GameWindow.hpp"
#include "Core/Components.hh"

#include "Systems/InputSystem.hpp"

InputSystem::InputSystem()
{
    addDependency<sInputComponent>();
    addDependency<sDirectionComponent>();
}

InputSystem::~InputSystem() {}

void    InputSystem::update(EntityManager &em, float elapsedTime)
{
    //auto &&keyboard = GameWindow::getInstance()->getKeyboard();
    //auto &&mouse = GameWindow::getInstance()->getMouse();

    forEachEntity(em, [&](Entity *entity) {
        movementKeys(entity, elapsedTime);
    });
}

void    InputSystem::movementKeys(Entity *entity, float elapsedTime)
{
    auto &&keyboard = GameWindow::getInstance()->getKeyboard();
    sInputComponent *input = entity->getComponent<sInputComponent>();
    sDirectionComponent *direction = entity->getComponent<sDirectionComponent>();

    direction->value = glm::vec2(0.0f, 0.0f);
    direction->moved = true;

    if (KB_P(LEFT))
    {
        direction->orientation.y += 180.0f * elapsedTime;
        direction->orientation.y = std::fmod(direction->orientation.y, 360.0f);
        //direction->orientation = glm::rotate(direction->orientation, 50.0f * elapsedTime, glm::vec3(0.0f, 1.0f, 0.0f));
    }
    else if (KB_P(RIGHT))
    {
        direction->orientation.y -= 180.0f * elapsedTime;
        direction->orientation.y = std::fmod(direction->orientation.y, 360.0f);
        //direction->orientation = glm::rotate(direction->orientation, -5.0f * elapsedTime, glm::vec3(0.0f, 1.0f, 0.0f));
    }
    if (KB_P(UP))
    {
        float angle = glm::radians(-direction->orientation.y + 90.0f);
        direction->value.x += glm::cos(angle);
        direction->value.y += glm::sin(angle);
    }
    else if (KB_P(DOWN))
    {
        float angle = glm::radians(-direction->orientation.y + 90.0f);
        direction->value.x -= glm::cos(angle);
        direction->value.y -= glm::sin(angle);
    }
}
