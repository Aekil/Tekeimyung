#pragma once

#include <iostream>
#include <memory>

#include <fmod.hpp>
#include "fmod_common.h"
#include "fmod_errors.h"

#define SOUND_MAX_CHANNELS  32

class SoundManager
{
public:
    SoundManager();
    virtual ~SoundManager();

    static std::shared_ptr<SoundManager>    getInstance();

    bool                                    initialize();
    void                                    update();
    void                                    shutdown();

    void                                    playBkgdMusic(const char *name, bool loop);

private:
    static std::shared_ptr<SoundManager>    _soundManager;

    FMOD_RESULT                             _result;
    FMOD::System*                           _system;
    FMOD::Channel*                          _channel;
    FMOD::Sound*                            _bkgdMusic;
};