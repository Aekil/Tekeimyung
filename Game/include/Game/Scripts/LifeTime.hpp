/*
** @Author : Simon AMBROISE
*/

#pragma once

#include <Engine/Core/BaseScript.hpp>
#include <Engine/Core/ScriptFactory.hpp>

class LifeTime final : public BaseScript
{
private:
    float _lifeTime;
    float _elapsedTime;

public:
    LifeTime() = default;
    ~LifeTime() = default;

    void start() override final;
    void update(float dt) override final;

public:
    bool updateEditor() override final;
    JsonValue saveToJson() override final;
    void loadFromJson(const JsonValue& json) override final;
};
