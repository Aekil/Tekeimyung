#pragma once

#include <memory>
#include <imgui.h>

#include <Engine/Utils/Debug.hpp>
#include <Engine/Utils/DebugWindow.hpp>

#include <Game/Utils/EventSound.hpp>

#define SOUNDEDITOR_WINDOW_TITLE    "Sound Editor"

class SoundEditorWindow : public DebugWindow
{
public:
    SoundEditorWindow(const glm::vec2& pos = glm::vec2(800, 100), const glm::vec2& size = glm::vec2(450, 450));
    virtual ~SoundEditorWindow();

    //static std::shared_ptr<SoundEditorWindow>   getInstance();

    virtual void                                build(float elapsedTime);


private:
    std::vector<tEventSound>                    _eventSoundData;
    //static std::shared_ptr<SoundEditorWindow>   _soundEditorWindow;
};

