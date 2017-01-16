/*
** @Author : Simon AMBROISE
*/

#pragma once

class Player
{
public:
    Player();
    void TakeDamage(int);
};

#include <Engine/Core/ScriptFactory.hpp>

class Trap : public BaseScript
{
protected:
    int damage;

public:
    Trap() {};
    ~Trap() {};
public:
    virtual void Start() = 0;
    virtual void Update(float dt) = 0;

    virtual void OnCollisionEnter(Entity* entity);
};