/**
* @Author   Simon Ambroise
*/

#pragma once

#include <ECS/System.hpp>

START_SYSTEM(MouseSystem)
public:
    MouseSystem();
    virtual ~MouseSystem() {};
    virtual void    update(EntityManager &em, float elapsedTime);

private:
    void hoverEntity(EntityManager&);

    uint32_t previousEntityId = 0;
END_SYSTEM(MouseSystem)
