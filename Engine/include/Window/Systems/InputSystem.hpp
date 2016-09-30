#pragma once

#include "System.hpp"

class InputSystem: public System
{
public:
    InputSystem();
    virtual ~InputSystem();
    virtual void update(EntityManager &em, float elapsedTime);
};