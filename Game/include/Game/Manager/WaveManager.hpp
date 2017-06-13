/*
** Author : Mathieu CHASSARA
*/

#pragma once

#include    <Engine/Core/BaseScript.hpp>
#include    <Engine/Core/ScriptFactory.hpp>

#include    <Game/Hud/ProgressBar.hpp>
#include    <Game/Manager/GoldManager.hpp>

#define WAVE_MANAGER_TAG    "WaveManager"

class       WaveManager final : public BaseScript
{
public:
    enum class  eState : int
    {
        STARTING = 0,
        ONGOING_WAVE,
        PENDING_WAVE,
        ENDING,
        ENDED
    };

    enum class  eBoardState : int
    {
        IDLE = 0,
        VICTORY,
        DEFEAT
    };
public:
    WaveManager() = default;
    ~WaveManager() override final = default;

    //  BaseScript.hpp override functions
public:
    void    start() override final;
    void    update(float dt) override final;

    void    setTutorialIsFinished(bool);

    void    handlePendingWave();

    //  Json serialization & ImGui edition.
public:
    bool        updateEditor() override final;
    JsonValue   saveToJson() override final;
    void        loadFromJson(const JsonValue& json) override final;

public:
    int     getCurrentWave() const;
    int     getNbWaves() const;
    eState  getManagerState() const;

private:
    void    retrieveGoldManager();

    void    startWave(uint32_t wave);
    bool    checkEndWave();
    bool    isGameOver();
    void    end();

    void    handleEndWave();
    void    handleGameOver();

    bool    checkBoardState(float deltaTime);
    void    spawnMapPart();
    void    updatePlayerState(bool teleport, unsigned int layer);
    void    updateProgressBar(float deltaTime);

private:
    unsigned int    _waves = 0;
    float           _timeBeforeWaveStarts = 0.0f;
    int             _currentWave = 0;
    eState          _state;
    eBoardState    _boardState;

    ProgressBar         _progressBar;
    std::vector<int>    _mapParts;

    GoldManager*        _goldManager = nullptr;
    bool                _tutorialIsFinished = false;
};

REGISTER_SCRIPT(WaveManager);
