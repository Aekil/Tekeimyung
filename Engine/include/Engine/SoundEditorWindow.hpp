/**
* @Author   Julien Chardon
*/

#pragma once

#include <memory>
#include <imgui.h>

#include <Engine/Utils/Debug.hpp>
#include <Engine/Utils/DebugWindow.hpp>

#include <Engine/Utils/EventSound.hpp>

#define SOUNDEDITOR_WINDOW_TITLE    "Sound Editor"

class SoundEditorWindow : public DebugWindow
{
public:
    SoundEditorWindow(const glm::vec2& pos = glm::vec2(800, 100), const glm::vec2& size = glm::vec2(450, 450));
    virtual ~SoundEditorWindow();

    void                        build(std::shared_ptr<GameState> gameState, float elapsedTime) override final;

    GENERATE_ID(SoundEditorWindow);

private:
    std::vector<tEventSound>    _eventSoundData;

    static std::string          _selectedSoundEventName;

    void                        buildSoundEventDetails(tEventSound* soundEvent, int soundEventIndex);
    //void                        displaySoundProgressLength(tEventSound* soundEvent);
};

