#pragma once

#include <memory>
#include <fmod.hpp>
#include <fmod_common.h>
#include <fmod_errors.h>

#define SOUND_MAX_CHANNELS  32
#define NB_MAX_SOUNDS       64

enum e_soundType { BACKGROUND_SOUND, DEFAULT_SOUND};

typedef struct s_sound
{
    unsigned int    id;
    bool            free;
    e_soundType     type;
    FMOD::Sound*    sound;
}                   t_sound;

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
    int                                     registerSound(const char *name, e_soundType type);
    void                                    playSound(unsigned int id);


    /*void                                    playBkgdMusic(const char *name, bool loop);
    void                                    playSound(const char *name);*/

private:
    static std::shared_ptr<SoundManager>    _soundManager;

    FMOD_RESULT                             _result;
    FMOD::System*                           _system;
    FMOD::Channel*                          _channel1;
    FMOD::Channel*                          _channel2;
    /*FMOD::Sound*                            _bkgdMusic;
    FMOD::Sound*                            _sound;*/
    t_sound                                 _sounds[NB_MAX_SOUNDS];
};