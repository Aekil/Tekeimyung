/*
** @Author : Guillaume Labey
*/

#pragma once

#include <Engine/Core/ScriptFactory.hpp>

class CheatCodesScript : public BaseScript
{
public:
    CheatCodesScript() = default;
    ~CheatCodesScript() = default;

    void start() override final;
    void update(float dt) override final;

    const Text& getText() const;

private:
    sTextComponent*   _textComp = nullptr;

    float _lastDelete = 0.0f;
};

REGISTER_SCRIPT(CheatCodesScript);
