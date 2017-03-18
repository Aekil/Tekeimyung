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
    virtual void onCollisionExit(Entity* entity) override final;

    void setTPPos(glm::vec3);
    void setActivity(bool);

private:
    glm::vec3 _tpPos;
    bool _enable{false};
};

REGISTER_SCRIPT(Teleport);
