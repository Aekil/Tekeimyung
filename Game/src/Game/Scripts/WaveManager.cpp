/*
** Author : Mathieu CHASSARA
*/

#include <Game/Scripts/WaveManager.hpp>

void        WaveManager::start()
{
    this->_waves = 4;
    this->_timeBeforeWaveStarts = 5.0f;
}

void        WaveManager::update(float dt)
{
    
}

int         WaveManager::getWaves() const
{
    return (this->_waves);
}

float       WaveManager::getTimeBeforeWaveStarts() const
{
    return (this->_timeBeforeWaveStarts);
}

void        WaveManager::setWaves(int waves)
{
    this->_waves = waves;
}

void        WaveManager::setTimeBeforeWaveStarts(float time)
{
    this->_timeBeforeWaveStarts = time;
}