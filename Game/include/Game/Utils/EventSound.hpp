#pragma once

#include <vector>
#include <unordered_map>

#define EVENT_SOUND(PROCESS)\
    PROCESS(BACKGROUND),\
    PROCESS(COUNTDOWN_WAVE),\
    PROCESS(BUILD),\

#define GENERATE_EVENTSOUND_ENUM(ENUM)                  ENUM
#define GENERATE_EVENTSOUND_STRING(STRING)              #STRING
#define GENERATE_EVENTSOUND_DATA(ENUM)                  { eEventSound::ENUM, #ENUM, -1 }


enum class eEventSound : uint16_t { EVENT_SOUND(GENERATE_EVENTSOUND_ENUM) };

typedef struct              sEventSound
{
    eEventSound             event;
    std::string             eventName;
    int                     soundID;
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
