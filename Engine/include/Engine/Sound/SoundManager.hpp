/**
* @Author   Julien Chardon
*/

#pragma once

#include <cstdint>
#include <memory>
#include <fmod.hpp>
#include <fmod_common.h>
#include <fmod_errors.h>

#if defined (ENGINE_DEBUG)
    #define ENABLE_SOUND    true
#else
    #define ENABLE_SOUND    true
#endif

#define SOUNDS_DIRECTORY    "resources/sounds"

#define NB_MAX_CHANNELS     64
#define NB_MAX_SOUNDS       64

#define DEFAULT_SOUND_VOL   1.0f

enum class eSoundType: char {
    /*NONE = 0,*/
    BACKGROUND_SOUND = 1,
    DEFAULT_SOUND = 2
};

typedef struct sSound
{
    int                 id;
    std::string         name;
    bool                free;
    bool                paused;
    float               volume;
    eSoundType          type;
    FMOD::Sound*        sound;
    FMOD::Channel*      channel;
}                       tSound;

/*typedef struct      sSoundInfos
{
    unsigned int    currentPosition;
    unsigned int    soundLength;

    unsigned int    currentMinutes;
    unsigned int    currentSeconds;

    unsigned int    lengthMinutes;
    unsigned int    lengthSeconds;
}                   tSoundInfos;*/

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

    void                                    freeSound(int id);
    int                                     registerSound(const std::string& name, eSoundType type/* = eSoundType::NONE*/);

    void                                    playSound(int id);
    void                                    resumeSound(int id);
    void                                    pauseSound(int id);
    void                                    stopSound(int id);

    void                                    restartSound(int id);

    bool                                    isSoundPlaying(int id);
    //bool                                    isSoundPaused(int id);

    /*unsigned int                            getSoundCurrentPosition(int id);
    unsigned int                            getSoundLength(int id);*/

    int                                     getSoundIDFromName(const std::string& name) const;
    const char*                             getSoundNameFromID(int id) const;

    void                                    pauseAllChannels();
    void                                    resumeAllChannels();
    void                                    setVolumeAllChannels(float volume);
    //void                                    setVolumeAllSfxChannels(float volume); // ! Danger ! : don't use this one 

    void                                    setSoundVolume(int id, float volume);

    //tSoundInfos                             getSoundInfos(uint32_t soundID);

    void                                    changeGeneralVolume(float volume);
    float                                   getGeneralVolume() const;

    void                                    setMuteState(bool muted);
    bool                                    getMuteState() const;

    void                                    setMuteBckgd(bool toMute);
    void                                    setMuteSfx(bool toMute);

private:
    /*void                                    addChannel(FMOD::Channel* channel);
    void                                    removeChannel(FMOD::Channel* channel);*/

private:
    static std::shared_ptr<SoundManager>    _soundManager;

    FMOD_RESULT                             _result;
    FMOD::System*                           _system;
    FMOD::ChannelGroup*                     _allChannelsGroup;
    //FMOD::ChannelGroup*                     _sfxChannelsGroup;

    tSound                                  _sounds[NB_MAX_SOUNDS];

    float                                   _generalVolume;
    bool                                    _muted;
};
