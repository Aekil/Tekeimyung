#pragma once

#include <vector>
#include <memory>
#include <fmod.hpp>
#include <fmod_common.h>
#include <fmod_errors.h>

#define ENABLE_SOUND        true

#define SOUNDS_DIRECTORY    "resources/sounds"

#define SOUND_MAX_CHANNELS  32
#define NB_MAX_SOUNDS       64

enum class eSoundType: char {
    NONE = 0,
    BACKGROUND_SOUND = 1,
    DEFAULT_SOUND = 2
};

typedef struct sSound
{
    unsigned int        id;
    std::string         name;
    bool                free;
    eSoundType          type;
    FMOD::Sound*        sound;
}                   tSound;

class SoundManager
{
public:
    SoundManager();
    ~SoundManager();

    static std::shared_ptr<SoundManager>    getInstance();

    bool                                    initialize();
    void                                    update();
    void                                    shutdown();

    bool                                    errorCheck();
    //std::string                             getErrorString();

    void                                    freeSound(unsigned int id);
    int                                     registerSound(const std::string& name, eSoundType type = eSoundType::NONE);
    void                                    playSound(unsigned int id);

    const char*                             getSoundNameFromID(int id) const;

private:
    static std::shared_ptr<SoundManager>    _soundManager;

    FMOD_RESULT                             _result;
    FMOD::System*                           _system;
    FMOD::Channel*                          _channel1;
    FMOD::Channel*                          _channel2;
    tSound                                  _sounds[NB_MAX_SOUNDS];
};
