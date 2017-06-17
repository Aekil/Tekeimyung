/**
* @Author   Julien CHARDON
*/

#pragma once

#include <unordered_map>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <Engine/Core/BaseScript.hpp>
#include <Engine/Core/ScriptFactory.hpp>
#include <Engine/Core/Components/Components.hh>

class GoldManager;
class WaveManager;

#define TUTO_MANAGER_TAG    "TutoManager"

enum class eTutoState: int
{
    MOVE,
    BUILD_BASE_TOWER,
    BUILD_TOWER,
    DEACTIVATE_BUILD,
    ENEMY_DEAD,
    BUILD_WALL,
    SHOOT,
    CHANGE_WEAPON,
    CHECK_HOWTOPLAY,
    CHECK_BUILDLIST,
    TUTO_DONE,
    TUTO_WAVE
};

eTutoState& operator++(eTutoState& state);
eTutoState  operator++(eTutoState& state, int);

class TutoManager final : public BaseScript
{
private:
    struct sTutoStep
    {
        eTutoState state;
        std::string message;
    };

public:
    TutoManager();
    ~TutoManager();

public:
    void start() override final;
    void update(float dt) override final;
    void sendMessage(eTutoState state);
    bool stateOnGoingOrDone(eTutoState state);
    bool stateOnGoing(eTutoState state);
    bool tutorialDone();

    eTutoState getCurrentState() const;

    static void display(bool displayed);

private:
    void spawnEnemy(float speed = -1.0f); // -1 does not change enemy speed
    void destroyBuilds();

    eTutoState getState(uint32_t stateIndex) const;

public:
    static bool         _tutorialDone;

private:
    GoldManager*        _goldManager;
    WaveManager*        _waveManager;
    sTextComponent*     _textComp = nullptr;
    uint32_t            _currentState = 0;

    std::vector<sTutoStep> _steps;
};
