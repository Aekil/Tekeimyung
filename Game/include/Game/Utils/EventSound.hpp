#pragma once

#include <vector>
#include <unordered_map>

#include <Engine/Sound/SoundManager.hpp>

#define EVENT_SOUND(PROCESS)\
    PROCESS(BACKGROUND, BACKGROUND_SOUND),\
    PROCESS(COUNTDOWN_WAVE, DEFAULT_SOUND),\
    PROCESS(BUILD, DEFAULT_SOUND),\

#define GENERATE_EVENTSOUND_ENUM(EVENT, SOUND_TYPE)     EVENT
#define GENERATE_EVENTSOUND_STRING(EVENT, SOUND_TYPE)   #EVENT
#define GENERATE_EVENTSOUND_DATA(EVENT, SOUND_TYPE)      { eEventSound::EVENT, #EVENT, -1, eSoundType::SOUND_TYPE }


enum class eEventSound : uint16_t { EVENT_SOUND(GENERATE_EVENTSOUND_ENUM) };

typedef struct              sEventSound
{
    eEventSound             event;
    std::string             eventName;
    int                     soundID;
    eSoundType              soundType;
    std::string             soundName;
}                           tEventSound;

class EventSound {
public:


public:
    EventSound();
    ~EventSound();

    static const std::vector<tEventSound>&  getEventSoundDataList();
    static int                              getSoundIDFromEvent(eEventSound event);

    static void                             setEventSoundID(int i, int id);

    static void                             linkEventSound(int i, int id, const std::string& name);

    static bool                             isEventLinkedToSound(eEventSound event);

private:
    static std::vector<tEventSound>         _eventSoundDataList;
};
