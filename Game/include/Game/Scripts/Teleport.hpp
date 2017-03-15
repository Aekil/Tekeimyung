/*
** @Author : Simon AMBROISE
*/

#pragma once

#include <Engine/Core/ScriptFactory.hpp>

class Teleport : public BaseScript
{
public:
    Teleport() {};
    ~Teleport() {};
public:
    virtual void start() override final;
    virtual void update(float dt) override final;

    virtual void onCollisionEnter(Entity* entity) override final;

    void setTPPos(glm::vec3);

private:
    glm::vec3 _tpPos;
};

REGISTER_SCRIPT(Teleport);
