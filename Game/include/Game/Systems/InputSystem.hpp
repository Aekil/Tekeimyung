#pragma once

#include <ECS/System.hpp>

#define LEFT            input->moveLeft
#define UP              input->moveUp
#define RIGHT           input->moveRight
#define DOWN            input->moveDown

#define KB_P(x)         keyboard.isPressed(x)
#define KB_ONLY_LEFT    (KB_P(LEFT) && !KB_P(RIGHT) && !KB_P(UP) && !KB_P(DOWN))
#define KB_ONLY_UP      (KB_P(UP) && !KB_P(RIGHT) && !KB_P(LEFT) && !KB_P(DOWN))
#define KB_ONLY_RIGHT   (KB_P(RIGHT) && !KB_P(LEFT) && !KB_P(UP) && !KB_P(DOWN))
#define KB_ONLY_DOWN    (KB_P(DOWN) && !KB_P(RIGHT) && !KB_P(UP) && !KB_P(LEFT))

class InputSystem: public System
{
public:
    InputSystem();
    virtual ~InputSystem();

    virtual void    update(EntityManager &em, float elapsedTime);

private:
    void            movementKeys(Entity *entity);
};
