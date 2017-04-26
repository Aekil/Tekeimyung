/*
** @Author : Simon AMBROISE
*/

#pragma once

#include <glm/vec3.hpp>
#include <Game/Scripts/ProgressBar.hpp>

class Health
{
public:
    Health() = default;
    virtual ~Health() = default;

public:
    virtual bool takeDamage(double damage);
    int getHealth() const;
    int getMaxHealth() const;
    void setHealth(int health);
    void setMaxHealth(int maxHealth);

protected:
    virtual void death() = 0;
    void init(sRenderComponent* render);
    void update(sTransformComponent* transform);

private:
    glm::vec3           _objCenter;
    ProgressBar         _progressBar;
};
