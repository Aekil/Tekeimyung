#include <imgui.h>

#include <Engine/Utils/Helper.hpp>
#include <Engine/Utils/Logger.hpp>
#include <Engine/Utils/RessourceManager.hpp>
#include <Engine/Sound/SoundManager.hpp>

#include <Game/SoundEditorWindow.hpp>

//std::shared_ptr<SoundEditorWindow>   SoundEditorWindow::_soundEditorWindow = nullptr;

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

void    SoundEditorWindow::build(float elapsedTime)
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
    auto soundsStrings = RessourceManager::getInstance()->getSoundsStrings();
    /*static*/ std::vector<tEventSound> eventSoundData /*= EventSound::getEventSoundDataList()*/;
    eventSoundData = instance.getEventSoundDataList();
    /*if (EventSound::isEventLinkedToSound(eEventSound::BACKGROUND))
    {
        int idtmp = EventSound::getSoundIDFromEvent(eEventSound::BACKGROUND);

    }*/
    //eventSoundData = EventSound::getEventSoundDataList();


    if (ImGui::Button("Save changes"))
    {
        EventSound::saveEvents();
    }

    ImGui::Text("Events :");
    ImGui::Separator();
    for (int i = 0; i < eventSoundData.size(); ++i)
    {
        ImGui::PushID(eventSoundData[i].eventName.c_str());
        ImGui::PushStyleColor(ImGuiCol_Button, ImColor(0.27f, 0.51f, 0.70f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor(0.39f, 0.58f, 0.92f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor(0.49f, 0.68f, 0.92f, 1.0f));
        ImGui::Text(eventSoundData[i].eventName.c_str());
        //LOG_DEBUG("event n%d | id = %d", i, eventSoundData[i].id);
        if (eventSoundData[i].soundID != -1) // if one sound is linked to the event
        {
            //LOG_INFO("SOUND NAME: %s", eventSoundData[i].soundName.c_str());
            ImGui::SameLine();
            ImGui::Text(" | ");
            ImGui::SameLine();
            ImGui::Text(eventSoundData[i].soundName.c_str());
        }
        ImGui::SameLine();
        if (ImGui::Button("Link sound"))
        {
            ImGui::OpenPopup("Sounds");
        }
        ImGui::PopStyleColor(3);
        if (ImGui::BeginPopup("Sounds"))
        {
            for (int j = 0; j < soundsStrings.size(); ++j)
            {
                if (ImGui::Button(soundsStrings[j].name.c_str()))
                {
                    // opti sound type
                    int id = SoundManager::getInstance()->registerSound(soundsStrings[j].path, eventSoundData[i].soundType);
                    if (id != -1)
                    {
                        EventSound::linkEventSound(i, id, soundsStrings[j]);
                        LOG_DEBUG("sound created : id = %d", id);
                    }
                    else
                    {
                        //EventSound::setEventSoundID(i, -1);
                        //eventSoundData[i].id = -1;
                        LOG_INFO("Can not create sound : %s", soundsStrings[j].name.c_str()); // log : info / warn ?
                    }
                }
            }
            ImGui::EndPopup();
        }
        ImGui::Separator();
        ImGui::PopID();
    }

    // when click on sound, create/link it and remember to display it side by the event

    ImGui::End();
}
