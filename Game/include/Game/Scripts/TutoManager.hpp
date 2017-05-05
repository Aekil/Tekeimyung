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
    TOWER_KILL_ENEMY = 3,
    BUILD_WALL = 4,
    CHECK_HOWTOPLAY = 5,
    CHECK_BUILDLIST = 6,
    SHOOT = 7,
    CHANGE_WEAPON = 8,
    TUTO_DONE = 9,
    TUTO_WAVE = 10
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
    void spawnEnemy();

private:
    WaveManager*        _waveManager;
    GoldManager*        _goldManager;
    sTextComponent*     _textComp = nullptr;
    eTutoState          _currentState = eTutoState::MOVE;

    std::unordered_map<eTutoState, std::string> _statesMessages;
};

REGISTER_SCRIPT(TutoManager);
