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

        if (keyboard.isPressed(input->moveLeft))
        {
            std::cout << "LEFT is " << (keyboard[input->moveLeft] == Keyboard::eKeyState::KEY_MAINTAINED ? "maintained" : "pressed") << " !" << std::endl;
            direction->x += -1.0;
            direction->y += 1.0;
        }

        if (keyboard.isPressed(input->moveRight))
        {
            std::cout << "RIGHT is " << (keyboard[input->moveRight] == Keyboard::eKeyState::KEY_MAINTAINED ? "maintained" : "pressed") << " !" << std::endl;
            direction->x += 1.0;
            direction->y += -1.0;
        }

        if (keyboard.isPressed(input->moveUp))
        {
            std::cout << "UP is " << (keyboard[input->moveUp] == Keyboard::eKeyState::KEY_MAINTAINED ? "maintained" : "pressed") << " !" << std::endl;
            direction->x += -1.0;
            direction->y += -1.0;
        }

        if (keyboard.isPressed(input->moveDown))
        {
            std::cout << "DOWN is " << (keyboard[input->moveDown] == Keyboard::eKeyState::KEY_MAINTAINED ? "maintained" : "pressed") << " !" << std::endl;
            direction->x += 1.0;
            direction->y += 1.0;
        }
    });
}