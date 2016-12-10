#pragma once

#include <vector>
#include <unordered_map>

#define EVENT_SOUND(PROCESS)\
    PROCESS(BACKGROUND),\
    PROCESS(COUNTDOWN_WAVE),\
    PROCESS(BUILD),\

#define GENERATE_EVENTSOUND_ENUM(ENUM)                  ENUM
#define GENERATE_EVENTSOUND_STRING(STRING)              #STRING
#define GENERATE_EVENTSOUND_DATA(ENUM)                  { eEventSound::ENUM, #ENUM, -1, nullptr }


enum class eEventSound { EVENT_SOUND(GENERATE_EVENTSOUND_ENUM) };

typedef struct              sEventSound
{
    eEventSound             event;
    const char*             eventName;
    int                     soundID;
    const char*             soundName;
}                           tEventSound;

class EventSound {
public:
    

public:
    EventSound();
    ~EventSound();

    static const std::vector<tEventSound>&  getEventSoundDataList();
    static int                              getSoundIDFromEvent(eEventSound event);
    
    static void                             setEventSoundID(int i, int id);

    static void                             linkEventSound(int i, int id, const char* name);

    static bool                             isEventLinkedToSound(eEventSound event);

private:
    static std::vector<tEventSound>         _eventSoundDataList;
};
