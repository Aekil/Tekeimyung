/**
* @Author   Julien Chardon
*/

#include <imgui.h>

#include <Engine/Utils/Helper.hpp>
#include <Engine/Utils/Logger.hpp>
#include <Engine/Utils/ResourceManager.hpp>
#include <Engine/Sound/SoundManager.hpp>

#include <Engine/SoundEditorWindow.hpp>

//std::shared_ptr<SoundEditorWindow>   SoundEditorWindow::_soundEditorWindow = nullptr;

std::string SoundEditorWindow::_selectedSoundEventName;

SoundEditorWindow::SoundEditorWindow(const glm::vec2& pos, const glm::vec2& size) :
    DebugWindow(SOUNDEDITOR_WINDOW_TITLE, pos, size)
{
    //eventSoundData = EventSound::getEventSoundDataList();
}

SoundEditorWindow::~SoundEditorWindow() {}

/*std::shared_ptr<SoundEditorWindow>   SoundEditorWindow::getInstance()
{
    if (!_soundEditorWindow)
    {
        _soundEditorWindow = std::make_shared<SoundEditorWindow>();
    }
    return (_soundEditorWindow);
}*/

void    SoundEditorWindow::build(std::shared_ptr<GameState> gameState, float elapsedTime)
{
    if (!ImGui::Begin(_title.c_str(), &_displayed, ImGuiWindowFlags_NoResize))
    {
        ImGui::End();
        return;
    }
    // Set Window params
    ImGui::SetWindowSize(ImVec2(_size.x, _size.y), ImGuiSetCond_Always);
    ImGui::SetWindowPos(ImVec2(_pos.x, _pos.y), ImGuiSetCond_Always);

    EventSound instance;
    auto soundsStrings = ResourceManager::getInstance()->getSoundsStrings();
    /*static*/ std::vector<tEventSound> eventSoundData /*= EventSound::getEventSoundDataList()*/;
    eventSoundData = instance.getEventSoundDataList();
    static int soundEventIndex = 0;
    /*if (EventSound::isEventLinkedToSound(eEventSound::BACKGROUND))
    {
        int idtmp = EventSound::getSoundIDFromEvent(eEventSound::BACKGROUND);

    }*/
    //eventSoundData = EventSound::getEventSoundDataList();


    if (ImGui::Button("Save changes"))
    {
        EventSound::saveEvents();
    }

    if (eventSoundData.size() == 0)
    {
        ImGui::Text("There is no sound event right now.");
    }
    else
    {
        ImGui::BeginChild("sound_events_list", ImVec2(150, 63), true);
        for (int index = 0; index < eventSoundData.size(); ++index)
        {
            ImGui::PushID(eventSoundData[index].eventName.c_str());
            if (ImGui::Selectable(eventSoundData[index].eventName.c_str(),
                _selectedSoundEventName == eventSoundData[index].eventName))
            {
                _selectedSoundEventName = eventSoundData[index].eventName;
                soundEventIndex = index;
            }
            ImGui::PopID();
        }
        ImGui::EndChild();
    }

    if (!SoundEditorWindow::_selectedSoundEventName.empty())
    {
        tEventSound* soundEvent = EventSound::getEventByName(_selectedSoundEventName);

        if (soundEvent == nullptr)
            LOG_WARN("Could not retrieve sound event by name (%s).", _selectedSoundEventName);
        else
            buildSoundEventDetails(soundEvent, soundEventIndex);
    }

    //ImGui::Text("Events :");
    //ImGui::Separator();
    //for (int i = 0; i < eventSoundData.size(); ++i)
    //{
    //    ImGui::PushID(eventSoundData[i].eventName.c_str());
    //    ImGui::PushStyleColor(ImGuiCol_Button, ImColor(0.27f, 0.51f, 0.70f, 1.0f));
    //    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor(0.39f, 0.58f, 0.92f, 1.0f));
    //    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor(0.49f, 0.68f, 0.92f, 1.0f));
    //    ImGui::Text(eventSoundData[i].eventName.c_str());
    //    //LOG_DEBUG("event n%d | id = %d", i, eventSoundData[i].id);
    //    if (eventSoundData[i].soundID != -1) // if one sound is linked to the event
    //    {
    //        //LOG_INFO("SOUND NAME: %s", eventSoundData[i].soundName.c_str());
    //        ImGui::SameLine();
    //        ImGui::Text(" | ");
    //        ImGui::SameLine();
    //        ImGui::Text(eventSoundData[i].soundName.c_str());
    //    }

    //    ImGui::SameLine();
    //    if (ImGui::Button("Link sound"))
    //    {
    //        ImGui::OpenPopup("Sounds");
    //    }

    //    if (eventSoundData[i].soundID != -1)
    //    {
    //        ImGui::SameLine();
    //        if (ImGui::Button("Remove sound"))
    //        {
    //            EventSound::removeEventSound(i);
    //        }
    //    }
    //    ImGui::PopStyleColor(3);

    //    if (ImGui::BeginPopup("Sounds"))
    //    {
    //        for (int j = 0; j < soundsStrings.size(); ++j)
    //        {
    //            if (ImGui::Button(soundsStrings[j].name.c_str()))
    //            {
    //                // opti sound type
    //                int id = SoundManager::getInstance()->registerSound(soundsStrings[j].path, eventSoundData[i].soundType);
    //                if (id != -1)
    //                {
    //                    EventSound::linkEventSound(i, id, soundsStrings[j]);
    //                }
    //                else
    //                {
    //                    //EventSound::setEventSoundID(i, -1);
    //                    //eventSoundData[i].id = -1;
    //                    LOG_INFO("Can not create sound : %s", soundsStrings[j].name.c_str()); // log : info / warn ?
    //                }
    //            }
    //        }
    //        ImGui::EndPopup();
    //    }
    //    ImGui::Separator();
    //    ImGui::PopID();
    //}

    // when click on sound, create/link it and remember to display it side by the event

    ImGui::End();
}

void        SoundEditorWindow::buildSoundEventDetails(tEventSound* soundEvent, int soundEventIndex)
{
    auto    soundsStrings = ResourceManager::getInstance()->getSoundsStrings();

    ImGui::BeginGroup();

    //  Building the details window about the selected sound event.
    if (ImGui::CollapsingHeader(soundEvent->eventName.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (!EventSound::isEventLinkedToSound(soundEvent->event))
        {
            if (ImGui::Button("Link a sound to this event"))
                ImGui::OpenPopup("Sounds");
        }
        else
        {
            ImGui::Text("Sound name:\t");
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "%s", soundEvent->soundName.c_str());
            ImGui::Text("Sound path:\t");
            ImGui::SameLine();
            ImGui::Text("%s", soundEvent->soundPath.c_str());
            if (ImGui::Button("Unlink the sound to this event"))
            {
                EventSound::removeEventSound(soundEventIndex);
            }
            displaySoundProgressLength(soundEvent); // Here, we display a cool progress bar.
            ImGui::Separator();
            if (!SoundManager::getInstance()->isSoundPaused(soundEvent->soundID) &&
                SoundManager::getInstance()->isSoundPlaying(soundEvent->soundID))
            {
                if (ImGui::Button("Pause"))
                {
                    SoundManager::getInstance()->pauseSound(soundEvent->soundID);
                }
                ImGui::SameLine();
                if (ImGui::Button("Stop"))
                {
                    SoundManager::getInstance()->stopSound(soundEvent->soundID);
                }
            }
            else
            {
                if (ImGui::Button("Play"))
                {
                    if (!SoundManager::getInstance()->isSoundPlaying(soundEvent->soundID))
                        SoundManager::getInstance()->playSound(soundEvent->soundID);
                    else
                        SoundManager::getInstance()->resumeSound(soundEvent->soundID);
                }
            }
        }

        //  Building the pop-up that lists all the sounds available.
        if (ImGui::BeginPopup("Sounds"))
        {
            for (int    index = 0; index < soundsStrings.size(); ++index)
            {
                if (ImGui::Button(soundsStrings[index].name.c_str()))
                {
                    int soundID = SoundManager::getInstance()->registerSound(soundsStrings[index].path, soundEvent->soundType);

                    if (soundID != -1)
                    {
                        EventSound::linkEventSound(soundEventIndex, soundID, soundsStrings[index]);
                    }
                    else
                        LOG_WARN("Could not register the specified sound (%s).",
                                 soundsStrings[index].name.c_str());
                }
            }
            ImGui::EndPopup();
        }
    }
    ImGui::EndGroup();
}

void                SoundEditorWindow::displaySoundProgressLength(tEventSound* soundEvent)
{
    tSoundInfos     soundInfos = SoundManager::getInstance()->getSoundInfos(soundEvent->soundID);
    char            buf[32];
    static float    progress;
    float           fraction;

    progress = 0.0f;
    fraction = (float) soundInfos.currentPosition / (float) soundInfos.soundLength;
    if (SoundManager::getInstance()->isSoundPlaying(soundEvent->soundID))
        progress += fraction;
    sprintf(buf, "%u:%u", soundInfos.currentMinutes, soundInfos.currentSeconds);

    //  The first argument of the ProgressBar must be included between 0.0f and 1.0f
    ImGui::ProgressBar(fraction, ImVec2(0.0f, 0.0f), buf);
    ImGui::SameLine();
    ImGui::Text("%u:%u", soundInfos.lengthMinutes, soundInfos.lengthSeconds);
}
