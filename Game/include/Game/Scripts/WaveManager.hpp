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
    ~WaveManager() = default;

private:
    int     _waves;
    float   _timeBeforeWaveStarts;

public:
    void    start() override final;
    void    update(float dt) override final;

public:
    int     getWaves() const;
    float   getTimeBeforeWaveStarts() const;

    void    setWaves(int waves);
    void    setTimeBeforeWaveStarts(float time);
};

REGISTER_SCRIPT(WaveManager);