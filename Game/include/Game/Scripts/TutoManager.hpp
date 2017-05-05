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

class GoldManager;
class WaveManager;

#define TUTO_MANAGER_TAG    "TutoManager"

enum class eTutoState: int
{
    MOVE = 0,
    BUILD_BASE_TOWER = 1,
    BUILD_TOWER = 2,
    BUILD_WALL = 3,
    CHECK_HOWTOPLAY = 4,
    CHECK_BUILDLIST = 5,
    SHOOT = 6,
    CHANGE_WEAPON = 7,
    TUTO_DONE = 8,
    TUTO_WAVE = 9
};

eTutoState& operator++(eTutoState& state);
eTutoState  operator++(eTutoState& state, int);

class TutoManager final : public BaseScript
{
public:
    TutoManager();
    ~TutoManager();

public:
    void start() override final;
    void update(float dt) override final;
    void sendMessage(eTutoState state);
    bool stateOnGoingOrDone(eTutoState state);

    static void display(bool displayed);

private:
    WaveManager*        _waveManager;
    GoldManager*        _goldManager;
    sTextComponent*     _textComp = nullptr;
    eTutoState          _currentState = eTutoState::MOVE;

    std::unordered_map<eTutoState, std::string> _statesMessages;
};

REGISTER_SCRIPT(TutoManager);
