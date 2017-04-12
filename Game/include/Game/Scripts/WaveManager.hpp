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
public:
    WaveManager() = default;
    ~WaveManager() override final = default;

public:
    void    start() override final;
    void    update(float dt) override final;
    bool    isWaiting();

    int     getCurrentWave() const;
    int     getNbWaves() const;

    //  Json serialization & ImGui edition.
public:
    bool        updateEditor() override final;
    JsonValue   saveToJson() override final;
    void        loadFromJson(const JsonValue& json) override final;

private:
    void    startWave(uint32_t wave);
    bool    checkEndWave();
    bool    checkGameOver();

    void    handleStartWave();
    void    handleEndWave();
    void    handleGameOver();

    bool    checkBoardState(float deltaTime);
    void    updatePlayerState(bool teleport, unsigned int layer);
    void    updateProgressBar(float deltaTime);

private:
    unsigned int        _waves;
    float               _timeBeforeWaveStarts;
    int                 _currentWave = 0;
    WaveManager::eState _state;

    ProgressBar         _progressBar;
    bool    _waiting{ false };
};

REGISTER_SCRIPT(WaveManager);
