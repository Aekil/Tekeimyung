/*
** Author : Mathieu CHASSARA
*/

#pragma once

#include    <Engine/Core/BaseScript.hpp>
#include    <Engine/Core/ScriptFactory.hpp>
#include    <Game/Scripts/ProgressBar.hpp>

class Build;

class       WaveManager final : public BaseScript
{
public:
    WaveManager() = default;
    ~WaveManager() override final = default;

public:
    void    start() override final;
    void    update(float dt) override final;
    bool    isWaiting();

private:
    void    startWave(uint32_t wave);
    bool    checkEndWave();
    bool    checkGameOver();

    void    handleStartWave();
    void    handleEndWave();
    void    handleGameOver();

private:
    int     _waves;
    float   _timeBeforeWaveStarts;
    int _currentWave{-1};

    ProgressBar _progressBar;
    bool    _waiting{false};

    Build* _playerBuild;
};

REGISTER_SCRIPT(WaveManager);
