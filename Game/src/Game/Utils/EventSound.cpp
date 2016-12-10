#include <Engine/Utils/Logger.hpp>

#include <Game/Utils/EventSound.hpp>

std::vector<tEventSound>  EventSound::_eventSoundDataList = { EVENT_SOUND(GENERATE_EVENTSOUND_DATA) };

EventSound::EventSound()
{}

EventSound::~EventSound()
{}

const std::vector<tEventSound>& EventSound::getEventSoundDataList()
{
    return (_eventSoundDataList);
}

int EventSound::getSoundIDFromEvent(eEventSound event)
{
    for (int i = 0; i < _eventSoundDataList.size(); ++i)
    {
        if (_eventSoundDataList[i].event == event)
        {
            return (_eventSoundDataList[i].soundID);
        }
    }
    return (-2); // not supposed to happen
}

void    EventSound::setEventSoundID(int i, int id)
{
    if (i < _eventSoundDataList.size())
    {
        _eventSoundDataList[i].soundID = id;/*
        if (id == -1)
            _eventSoundDataList[i].soundName = "none";*/
    }
    else
    {
        LOG_WARN("Event Sound class : setEventSoundID() : incorrect index value");
    }
}

void    EventSound::linkEventSound(int i, int id, const std::string& name)
{
    EventSound::setEventSoundID(i, id);
    _eventSoundDataList[i].soundName = name;
}

bool    EventSound::isEventLinkedToSound(eEventSound event)
{
    for (int i = 0; i < _eventSoundDataList.size(); ++i)
    {
        if (_eventSoundDataList[i].event == event && _eventSoundDataList[i].soundID != -1)
        {
            return (true);
        }
    }
    return (false);
}
