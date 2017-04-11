/*
** Author : Mathieu CHASSARA
*/

#pragma once

#include    <Engine/Core/BaseScript.hpp>
#include    <Engine/Core/ScriptFactory.hpp>
#include    <Game/Scripts/ProgressBar.hpp>

#define WAVE_MANAGER_TAG    "WaveManager"

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

    int     getCurrentWave() const;
    int     getNbWaves() const;

    bool updateEditor() override final;
    JsonValue saveToJson() override final;
    void loadFromJson(const JsonValue& json) override final;

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
    int _currentWave{ -1 };

    ProgressBar _progressBar;
    bool    _waiting{ false };

    Build* _playerBuild;

    std::vector<int> _mapParts;
};

REGISTER_SCRIPT(WaveManager);
