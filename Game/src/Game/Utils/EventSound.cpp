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

void    EventSound::setEventSoundID(int i, int id)
{
    if (i < _eventSoundDataList.size())
    {
        _eventSoundDataList[i].soundID = id;
    }
    else
    {
        LOG_WARN("Event Sound class : setEventSoundID() : incorrect index value");
    }
}

void    EventSound::linkEventSound(int i, int id, const RessourceManager::sSoundStrings& soundString)
{
    EventSound::setEventSoundID(i, id);
    _eventSoundDataList[i].soundName = soundString.name;
    _eventSoundDataList[i].soundPath = soundString.path;
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
