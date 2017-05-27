/*
** @Author : Simon AMBROISE
*/

#pragma once

#include <glm/vec3.hpp>

#include <Engine/Core/Components/Components.hh>
#include <Engine/Core/ScriptFactory.hpp>
#include <Engine/Utils/EventSound.hpp>

#include <Game/Attibutes/Attribute.hpp>
#include <Game/Scripts/Health.hpp>

#define ENEMY_TAG   "Enemy"

class Enemy final : public BaseScript, public Health
{
    friend class TutoManager;

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

    void stun(float dt);

    Attribute* getPercentExplosion();
    void setPercentExplosion(Attribute*);
    void setPath(const std::vector<glm::vec3>& path);

    bool updateEditor() override final;
    JsonValue saveToJson() override final;
    void loadFromJson(const JsonValue& json) override final;

private:
    void resetBloom();
    bool updateStun(float dt);
    void followPath(float dt);
    void destroyWall(Entity* entity, const glm::vec3& pos);

private:
    Entity* _healthBarEmpty;
    sTransformComponent* _healthBarEmptyTransform;
    Entity* _healthBar;
    sTransformComponent* _healthBarTransform;

    sTransformComponent* _transform;
    sRenderComponent* _render;
    sRenderComponent* _healthRender;

    sRigidBodyComponent* _rigidBody;

    std::vector<glm::vec3> _path;
    uint32_t _pathProgress{0};

    tEventSound* _dyingSound = nullptr;
    tEventSound* _earningCoins = nullptr;

    Attribute* _percentExplosion = nullptr;
    float _stun{0.0f};

    float   _speed = 0.0f;
    int     _goldEarned = 0;
    int     _experienceEarned = 0;

    std::vector<glm::vec4> _baseBlooms;
};

REGISTER_SCRIPT(Enemy);
