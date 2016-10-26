#pragma once

#include <iostream>

#include <fmod.hpp>

#define SOUND_MAX_CHANNELS  512

class SoundManager // put in singleton ?
{
public:
    SoundManager();
    virtual ~SoundManager();

    int                 initialize();
    void                update();
    void                shutdown();

    void                playBkgdMusic(const char *name, bool loop);

private:
    FMOD_RESULT         _result;
    FMOD::System*       _system;
    //FMOD::Sound**       _bkgdMusic;
};