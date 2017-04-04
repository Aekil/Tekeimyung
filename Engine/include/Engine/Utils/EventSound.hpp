/**
* @Author   Julien Chardon
*/

#pragma once

#include <vector>
#include <unordered_map>
#include <utility>

#include <Engine/Utils/ResourceManager.hpp>

#include <Engine/Sound/SoundManager.hpp>

#define EVENTS_CONFIG "resources/sounds/sound_events.json"

#define EVENT_SOUND(PROCESS)\
    PROCESS(BACKGROUND, BACKGROUND_SOUND),\
    PROCESS(BUILD, DEFAULT_SOUND),\
    PROCESS(ENEMY_DYING, DEFAULT_SOUND),\
    PROCESS(TOWER_SHOOT, DEFAULT_SOUND),\
    PROCESS(PLAYER_SHOOT, DEFAULT_SOUND),\
    PROCESS(EARN_COINS_FROM_ENEMY, DEFAULT_SOUND),\
    

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
    std::string             soundPath;
}                           tEventSound;

class EventSound {
public:


public:
    EventSound();
    ~EventSound();

    static const std::vector<tEventSound>&  getEventSoundDataList();
    static int                              getSoundIDFromEvent(eEventSound event);

    static void                             linkEventSound(int i, int id, const ResourceManager::sSoundStrings& soundString);
    static void                             removeEventSound(int i);

    static bool                             isEventLinkedToSound(eEventSound event);

    static void                             loadEvents();
    static void                             saveEvents();

    static tEventSound*                     getEventBySoundPath(const std::string& soundPath);
    static tEventSound*                     getEventByEventType(const eEventSound& event);
    static tEventSound*                     getEventByName(const std::string& name);

private:
    static std::vector<tEventSound>         _eventSoundDataList;
};
