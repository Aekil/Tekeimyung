#pragma once

#include <cstdint>
#include <memory>
#include <fmod.hpp>
#include <fmod_common.h>
#include <fmod_errors.h>

#define ENABLE_SOUND        true

#define SOUNDS_DIRECTORY    "resources/sounds"

#define NB_MAX_CHANNELS     32
#define NB_MAX_SOUNDS       64

enum class eSoundType: char {
    NONE = 0,
    BACKGROUND_SOUND = 1,
    DEFAULT_SOUND = 2
};

typedef struct sSound
{
    uint32_t        id;
    std::string         name;
    bool                free;
    eSoundType          type;
    FMOD::Sound*        sound;
    FMOD::Channel*      channel;
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

    void                                    freeSound(uint32_t id);
    int                                     registerSound(const std::string& name, eSoundType type = eSoundType::NONE);

    void                                    playSound(uint32_t id);
    void                                    stopSound(uint32_t id);

    bool                                    isSoundPlaying(uint32_t id);

    const char*                             getSoundNameFromID(int id) const;

    void                                    pause();
    void                                    resume();
    void                                    setVolume(float volume);

private:
    static std::shared_ptr<SoundManager>    _soundManager;

    FMOD_RESULT                             _result;
    FMOD::System*                           _system;

    tSound                                  _sounds[NB_MAX_SOUNDS];
    FMOD::Channel*                          _channels[NB_MAX_CHANNELS];
};
