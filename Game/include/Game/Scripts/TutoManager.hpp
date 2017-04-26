/**
* @Author   Julien CHARDON
*/

#pragma once

#include <unordered_map>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <Engine/Core/BaseScript.hpp>
#include <Engine/Core/ScriptFactory.hpp>
#include <Engine/Components.hh>

#define TUTO_MANAGER_TAG    "TutoManager"

enum class eTutoState: int
{
    MOVE = 0,
    SHOOT = 1,
    CHANGE_WEAPON = 2,
    CHOOSE_BUILD = 3,
    BUILD = 4,
    DISABLE_BUILD = 5,
    CHECK_HOWTOPLAY = 6,
    CHECK_BUILDLIST = 7,
    TUTO_DONE = 8,
    TUTO_HIDDEN = 9
};

eTutoState& operator++(eTutoState& state);
eTutoState operator++(eTutoState& state, int);

class TutoManager final : public BaseScript
{
public:
    TutoManager();
    ~TutoManager() = default;

public:
    void start() override final;
    void update(float dt) override final;
    void sendMessage(eTutoState state);
    bool isFinished();

private:
    sTextComponent*     _textComp = nullptr;

    eTutoState _currentState = eTutoState::MOVE;

    std::unordered_map<eTutoState, std::string> _statesMessages;

    bool _isFinished = false;
};

REGISTER_SCRIPT(TutoManager);
