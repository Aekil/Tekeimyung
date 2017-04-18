#pragma once

#include <Engine/Core/BaseScript.hpp>
#include <Engine/Core/ScriptFactory.hpp>

#include <Game/Scripts/Health.hpp>

#define CASTLE_TAG          "Castle"

#define MAX_CASTLE_HEALTH   4200
#define DMG_FOR_EACH_ENEMY  200

class Castle final : public BaseScript, public Health
{
public:
    Castle() = default;
    ~Castle() = default;

public:
    void start() override final;
    void update(float dt) override final;
    void onCollisionEnter(Entity* entity) override final;
    void death() override final;

    //  Json serialization & ImGui edition.
public:
    bool        updateEditor() override final;
    JsonValue   saveToJson() override final;
    void        loadFromJson(const JsonValue& json) override final;

private:
    Entity*                 _healthBarEmpty;
    sTransformComponent*    _healthBarEmptyTransform;

    Entity*                 _healthBar;
    sTransformComponent*    _healthBarTransform;

    sTransformComponent*    _transform;
    sRenderComponent*       _render;
    sRenderComponent*       _healthRender;
};

REGISTER_SCRIPT(Castle);
