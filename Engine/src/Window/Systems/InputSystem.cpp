#include "Window/GameWindow.hpp"
#include "Core/Components.hh"

#include "Window/Systems/InputSystem.hpp"

InputSystem::InputSystem()
{
    addDependency<sInputComponent>();
    addDependency<sDirectionComponent>();
}

InputSystem::~InputSystem() {}

void    InputSystem::update(EntityManager &em, float elapsedTime)
{
    auto &&keyboard = GameWindow::getInstance()->getKeyboard();
    auto &&mouse = GameWindow::getInstance()->getMouse();

    forEachEntity(em, [&](Entity *entity) {
        sInputComponent *input = entity->getComponent<sInputComponent>();
        sDirectionComponent *direction = entity->getComponent<sDirectionComponent>();

        direction->value = glm::vec2(0.0f, 0.0f);

        direction->moved = false;

        if (keyboard.isPressed(input->moveLeft))
        {
            direction->value.x += -1.0;
            direction->value.y += 1.0;
            direction->moved = true;
            direction->orientation = eOrientation::W;
        }

        if (keyboard.isPressed(input->moveRight))
        {
            direction->value.x += 1.0;
            direction->value.y += -1.0;
            direction->moved = true;
            direction->orientation = eOrientation::E;
        }

        if (keyboard.isPressed(input->moveUp))
        {
            direction->value.x += -1.0;
            direction->value.y += -1.0;
            direction->moved = true;

            if (keyboard.isPressed(input->moveLeft))
                direction->orientation = eOrientation::NW;
            else if (keyboard.isPressed(input->moveRight))
                direction->orientation = eOrientation::NE;
            else
                direction->orientation = eOrientation::N;
        }

        if (keyboard.isPressed(input->moveDown))
        {
            direction->value.x += 1.0;
            direction->value.y += 1.0;
            direction->moved = true;

            if (keyboard.isPressed(input->moveLeft))
                direction->orientation = eOrientation::SW;
            else if (keyboard.isPressed(input->moveRight))
                direction->orientation = eOrientation::SE;
            else
                direction->orientation = eOrientation::S;
        }
    });
}
