/*
** @Author : Simon AMBROISE
*/

#pragma once

#include <glm/vec3.hpp>
#include <Game/Scripts/ProgressBar.hpp>

class Health: public ProgressBar
{
public:
    Health() = default;
    virtual ~Health() override = default;

public:
    virtual bool takeDamage(int damage);
    int getHealth() const;
    void setHealth(int health);
    void setMaxHealth(int maxHealth);

protected:
    virtual void death() = 0;
    void init(sRenderComponent* render);
    void update(sTransformComponent* transform);

private:
    sRenderComponent*   _healthRender{nullptr};
    glm::vec3           _objCenter;
};
