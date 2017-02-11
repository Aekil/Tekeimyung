/*
** @Author : Simon AMBROISE
*/

#pragma once

class Health
{
public:
    Health() {};
    ~Health() {};

public:
    bool TakeDamage(int);

protected:
    int health;
    virtual void Death() = 0;
};
