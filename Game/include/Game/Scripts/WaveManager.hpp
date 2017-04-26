/*
** Author : Mathieu CHASSARA
*/

#pragma once

#include    <Engine/Core/BaseScript.hpp>
#include    <Engine/Core/ScriptFactory.hpp>
#include    <Game/Scripts/ProgressBar.hpp>

#define WAVE_MANAGER_TAG    "WaveManager"

class       Build;

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

public:
    void    start() override final;
    void    update(float dt) override final;

    int     getCurrentWave() const;
    int     getNbWaves() const;
    eState  getManagerState() const;

    //  Json serialization & ImGui edition.
public:
    bool        updateEditor() override final;
    JsonValue   saveToJson() override final;
    void        loadFromJson(const JsonValue& json) override final;

private:
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

    ProgressBar    _progressBar;
    Build* _playerBuild;
    std::vector<int> _mapParts;
};

REGISTER_SCRIPT(WaveManager);
