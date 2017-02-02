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
    void TakeDamage(int);

protected:
    int health;
    virtual void Death() = 0;
};