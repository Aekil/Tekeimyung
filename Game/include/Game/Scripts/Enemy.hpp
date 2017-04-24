/*
** @Author : Simon AMBROISE
*/

#pragma once

#include <glm/vec3.hpp>

#include <Engine/Core/ScriptFactory.hpp>
#include <Engine/Utils/EventSound.hpp>

#include <Game/Scripts/Health.hpp>
#include <Game/Attibutes/Attribute.hpp>

#define ENEMY_TAG   "Enemy"

class Enemy final : public BaseScript, public Health
{
public:
    Enemy() = default;
    ~Enemy() override final = default;

public:
    void start() override final;
    void update(float dt) override final;
    void onCollisionEnter(Entity* entity) override final;
    void death() override final;
    void remove();
    bool takeDamage(double damage) override final;

    Attribute* getPercentExplosion();
    void setPercentExplosion(Attribute*);
    void setPath(const std::vector<glm::vec3>& path);

    bool updateEditor() override final;
    JsonValue saveToJson() override final;
    void loadFromJson(const JsonValue& json) override final;

private:
    Entity* _healthBarEmpty;
    sTransformComponent* _healthBarEmptyTransform;
    Entity* _healthBar;
    sTransformComponent* _healthBarTransform;

    sTransformComponent* _transform;
    sRenderComponent* _render;
    sRenderComponent* _healthRender;

    std::vector<glm::vec3> _path;
    uint32_t _pathProgress{0};

    tEventSound* _dyingSound = nullptr;
    tEventSound* _earningCoins = nullptr;

    Attribute* _percentExplosion = nullptr;

    float _speed;

    int _experienceEarned;
};

REGISTER_SCRIPT(Enemy);
