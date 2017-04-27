/**
* @Author   Julien Chardon
*/

#pragma once

#include <memory>
#include <imgui.h>

#include <Engine/Debug/Debug.hpp>
#include <Engine/Debug/DebugWindow.hpp>

#include <Engine/Utils/EventSound.hpp>

#define SOUNDEDITOR_WINDOW_TITLE    "Sound Editor"

class SoundEditorWindow : public DebugWindow
{
public:
    SoundEditorWindow();
    SoundEditorWindow(const ImVec2& pos, const ImVec2& size);
    virtual ~SoundEditorWindow();

    void                        build(std::shared_ptr<GameState> gameState, float elapsedTime) override final;

    GENERATE_ID(SoundEditorWindow);

private:
    std::vector<tEventSound>    _eventSoundData;

    static std::string          _selectedSoundEventName;

    void                        buildSoundEventDetails(tEventSound* soundEvent, int soundEventIndex);
    //void                        displaySoundProgressLength(tEventSound* soundEvent);
};

