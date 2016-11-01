#pragma once

#include <memory>
#include <fmod.hpp>
#include <fmod_common.h>
#include <fmod_errors.h>

#define SOUND_MAX_CHANNELS  32
#define NB_MAX_SOUNDS       64

enum eSoundType { BACKGROUND_SOUND, DEFAULT_SOUND};

typedef struct sSound
{
    unsigned int    id;
    bool            free;
    eSoundType      type;
    FMOD::Sound*    sound;
}                   tSound;

class SoundManager
{
public:
    SoundManager();
    virtual ~SoundManager();

    static std::shared_ptr<SoundManager>    getInstance();

    bool                                    initialize();
    void                                    update();
    void                                    shutdown();

    bool                                    errorCheck();
    //std::string                             getErrorString();

    void                                    freeSound(unsigned int id);
    int                                     registerSound(const char *name, eSoundType type);
    void                                    playSound(unsigned int id);

private:
    static std::shared_ptr<SoundManager>    _soundManager;

    FMOD_RESULT                             _result;
    FMOD::System*                           _system;
    FMOD::Channel*                          _channel1;
    FMOD::Channel*                          _channel2;
    tSound                                  _sounds[NB_MAX_SOUNDS];
};