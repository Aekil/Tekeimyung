/*
** @Author : Simon AMBROISE
*/

#pragma once

#include <glm/vec3.hpp>

struct sTransformComponent;
struct sRenderComponent;
class Entity;

class Health
{
public:
    Health();
    virtual ~Health();

public:
    bool TakeDamage(int);

protected:
    int health;
    int maxHealth;
    virtual void Death() = 0;
    void init(sTransformComponent* transform, sRenderComponent* render);
    void update(sTransformComponent* transform);

private:
    Entity* _healthBarEmpty;
    sTransformComponent* _healthBarEmptyTransform;
    Entity* _healthBar;
    sTransformComponent* _healthBarTransform;
    sRenderComponent* _healthRender;
    glm::vec3 _objCenter;

    float maxScale;
};
