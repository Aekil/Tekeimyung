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

    forEachEntity(em, [&](Entity *entity) {
        sInputComponent *input = entity->getComponent<sInputComponent>();
        sDirectionComponent *direction = entity->getComponent<sDirectionComponent>();

        direction->x = 0;
        direction->y = 0;

        if (keyboard[input->moveLeft] != Keyboard::KeyState::KEY_RELEASED) {
            direction->x += -1.0;
            direction->y += 1.0;
        }
        if (keyboard[input->moveRight] != Keyboard::KeyState::KEY_RELEASED) {
            direction->x += 1.0;
            direction->y += -1.0;
        }
        if (keyboard[input->moveUp] != Keyboard::KeyState::KEY_RELEASED) {
            direction->x += -1.0;
            direction->y += -1.0;
        }
        if (keyboard[input->moveDown] != Keyboard::KeyState::KEY_RELEASED) {
            direction->x += 1.0;
            direction->y += 1.0;
        }
    });
}