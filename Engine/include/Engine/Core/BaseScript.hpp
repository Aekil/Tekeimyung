/**
* @Author   Simon AMBROISE
*/

#pragma once

#include <ECS/Entity.hpp>

class BaseScript
{
public:
    BaseScript();
    virtual ~BaseScript() {};
    virtual void Start() = 0;
    virtual void Update(float dt) = 0;

    virtual void OnCollisionEnter(int entityId) {};
    virtual void OnCollisionExit(int entityId) {};
};