#include <Engine/Utils/Logger.hpp>
#include <Engine/Utils/RessourceManager.hpp>
#include <Engine/Utils/Exception.hpp>
#include <Engine/Sound/SoundManager.hpp>

#include <Game/Utils/JsonWriter.hpp>
#include <Game/Utils/JsonReader.hpp>

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
    ASSERT(false, "EventSound::getSoundIDFromEvent() not supposed to happen");
    return (-2); // not supposed to happen
}

void    EventSound::linkEventSound(int i, int id, const RessourceManager::sSoundStrings& soundString)
{
    // Out of range
    if (i < 0 || i >= _eventSoundDataList.size())
    {
        LOG_WARN("EventSound::setEventSoundID() : incorrect index value \"%d\"", i);
    }
    else
    {
        _eventSoundDataList[i].soundID = id;
        _eventSoundDataList[i].soundName = soundString.name;
        _eventSoundDataList[i].soundPath = soundString.path;
    }
}

void    EventSound::removeEventSound(int i)
{
    // Out of range
    if (i < 0 || i >= _eventSoundDataList.size())
    {
        LOG_WARN("EventSound::removeEventSound() : incorrect index value \"%d\"", i);
    }
    else
    {
        SoundManager::getInstance()->stopSound(_eventSoundDataList[i].soundID);
        _eventSoundDataList[i].soundID = -1;
        _eventSoundDataList[i].soundName = "";
        _eventSoundDataList[i].soundPath = "";
    }
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

void    EventSound::loadEvents()
{
    try
    {
        JsonReader jsonReader;
        JsonValue json;
        const std::string& eventsFileContent = RessourceManager::getInstance()->getFile(EVENTS_CONFIG);
        if (!jsonReader.parse(eventsFileContent, json))
            EXCEPT(IOException, "Cannot parse event file \"%s\"", EVENTS_CONFIG);

        auto events = json.get()["events"];
        if (events.size() > 0 && events.type() != Json::ValueType::arrayValue)
        {
            EXCEPT(InvalidParametersException, "EventSound::loadEvents: load json \"%s\" error: events is not an array", EVENTS_CONFIG);
        }

        for (auto& event: events)
        {
            JsonValue eventJson(event);
            JsonValue soundJson(eventJson.get()["sound"]);

            tEventSound* eventSound = getEventByName(eventJson.getString("name", ""));
            if (eventSound != nullptr && soundJson.size() > 0)
            {
                eventSound->soundName = soundJson.getString("name", "");
                eventSound->soundPath = soundJson.getString("path", "");

                if (eventSound->soundPath.size() > 0)
                    eventSound->soundID = SoundManager::getInstance()->registerSound(eventSound->soundPath, eventSound->soundType);
            }
        }
    }
    catch (...)
    {
        return;
    }
}

void    EventSound::saveEvents()
{
    JsonWriter jsonWriter;
    JsonValue json;
    std::vector<JsonValue> events;

    for (tEventSound& event: _eventSoundDataList)
    {
        JsonValue eventJson;
        JsonValue soundJson;

        // Event name
        eventJson.setString("name", event.eventName);

        // Sound infos
        soundJson.setString("path", event.soundPath);
        soundJson.setString("name", event.soundName);

        // Add sound field to event
        eventJson.setValue("sound", soundJson);
        events.push_back(eventJson);
    }

    json.setValueVec("events", events);

    jsonWriter.write(EVENTS_CONFIG, json);

    LOG_INFO("Sound events saved");
}

tEventSound*    EventSound::getEventByName(const std::string& name)
{
    for (int i = 0; i < _eventSoundDataList.size(); ++i)
    {
        if (_eventSoundDataList[i].eventName == name)
        {
            return (&_eventSoundDataList[i]);
        }
    }

    return (nullptr);
}

tEventSound*    EventSound::getEventBySoundPath(const std::string& soundPath)
{
    for (int i = 0; i < _eventSoundDataList.size(); ++i)
    {
        if (_eventSoundDataList[i].soundPath == soundPath)
        {
            return (&_eventSoundDataList[i]);
        }
    }

    return (nullptr);
}

tEventSound*    EventSound::getEventByEventType(const eEventSound& event)
{
    for (int i = 0; i < _eventSoundDataList.size(); ++i)
    {
        if (_eventSoundDataList[i].event == event)
        {
            return (&_eventSoundDataList[i]);
        }
    }

    return (nullptr);
}
