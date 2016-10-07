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

        direction->x = 0;
        input->keyPressed = false;
        direction->y = 0;

        if (keyboard.isPressed(input->moveLeft))
        {
            //std::cout << "LEFT is " << (keyboard[input->moveLeft] == Keyboard::eKeyState::KEY_MAINTAINED ? "maintained" : "pressed") << " !" << std::endl;
            direction->x += -1.0;
            direction->y += 1.0;
            input->keyPressed = true;
            direction->orientation = eOrientation::W;
        }

        if (keyboard.isPressed(input->moveRight))
        {
            //std::cout << "RIGHT is " << (keyboard[input->moveRight] == Keyboard::eKeyState::KEY_MAINTAINED ? "maintained" : "pressed") << " !" << std::endl;
            direction->x += 1.0;
            direction->y += -1.0;
            input->keyPressed = true;
            direction->orientation = eOrientation::E;
        }

        if (keyboard.isPressed(input->moveUp))
        {
            //std::cout << "UP is " << (keyboard[input->moveUp] == Keyboard::eKeyState::KEY_MAINTAINED ? "maintained" : "pressed") << " !" << std::endl;
            direction->x += -1.0;
            direction->y += -1.0;
            input->keyPressed = true;

            if (keyboard.isPressed(input->moveLeft))
                direction->orientation = eOrientation::NW;
            else if (keyboard.isPressed(input->moveRight))
                direction->orientation = eOrientation::NE;
            else
                direction->orientation = eOrientation::N;
        }

        if (keyboard.isPressed(input->moveDown))
        {
            //std::cout << "DOWN is " << (keyboard[input->moveDown] == Keyboard::eKeyState::KEY_MAINTAINED ? "maintained" : "pressed") << " !" << std::endl;
            direction->x += 1.0;
            direction->y += 1.0;
            input->keyPressed = true;

            if (keyboard.isPressed(input->moveLeft))
                direction->orientation = eOrientation::SW;
            else if (keyboard.isPressed(input->moveRight))
                direction->orientation = eOrientation::SE;
            else
                direction->orientation = eOrientation::S;
        }

        //if (mouse.isPressed(Mouse::eButton::MOUSE_BUTTON_1))
            //std::cout << "BUTTON is " << (mouse.getStateMap()[Mouse::eButton::MOUSE_BUTTON_1] == Mouse::eButtonState::CLICK_MAINTAINED ? "maintained" : "pressed") << " !" << std::endl;

        //std::cout << "Cursor is " << (mouse.getCursor().isInTheWindow() ? "in the window" : "is out of the window") << " (" << mouse.getCursor().getX() << ", " << mouse.getCursor().getY() << ")" << std::endl;
    });
}