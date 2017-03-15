/*
** Author : Mathieu CHASSARA
*/

#pragma once

#include    <Engine/Core/BaseScript.hpp>
#include    <Engine/Core/ScriptFactory.hpp>

class       WaveManager final : public BaseScript
{
public:
    WaveManager() = default;
    ~WaveManager() override final = default;

public:
    void    start() override final;
    void    update(float dt) override final;

    int     getWaves() const;
    float   getTimeBeforeWaveStarts() const;

    void    setWaves(int waves);
    void    setTimeBeforeWaveStarts(float time);

private:
    void    startWave(uint32_t wave);
    bool    checkEndWave();

private:
    int     _waves;
    float   _timeBeforeWaveStarts;
    int _currentWave{-1};
};

REGISTER_SCRIPT(WaveManager);
