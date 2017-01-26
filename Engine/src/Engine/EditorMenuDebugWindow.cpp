/**
* @Author   Guillaume Labey
*/

#include <imgui.h>

#include <Engine/EntityFactory.hpp>
#include <Engine/EditorMenuDebugWindow.hpp>


EditorMenuDebugWindow::EditorMenuDebugWindow(EntityManager* em, const glm::vec2& pos, const glm::vec2& size):
                                    _em(em), DebugWindow("", pos, size) {}

EditorMenuDebugWindow::~EditorMenuDebugWindow() {}

void    EditorMenuDebugWindow::build(float elapsedTime)
{
    if (!ImGui::Begin(_title.c_str(), &_displayed, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize))
    {
        ImGui::End();
        return;
    }
    // Set Window params
    ImGui::SetWindowPos(ImVec2(_pos.x, _pos.y), ImGuiSetCond_Always);

    bool load = false;
    bool save = false;
    bool saveAs = false;

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Levels"))
        {
            if (ImGui::MenuItem("Load"))
            {

            }
            else if (ImGui::MenuItem("Save"))
            {
                save = true;
            }
            else if (ImGui::MenuItem("Save as"))
            {
                saveAs = true;
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    if (saveAs || (save && _currentLevel.size() == 0))
    {
        ImGui::OpenPopup("Save level as...");
    }
    else if (save)
    {
        ImGui::OpenPopup("Save level");
    }

    if (ImGui::BeginPopup("Save level as..."))
    {
        static char levelName[64] = "";
        ImGui::InputText("Level name", levelName, 64);
        if (ImGui::Button("Save"))
        {
            _currentLevel = levelName;
            _levelLoader.save(_currentLevel, _em->getEntities());
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
    else if (ImGui::BeginPopup("Save level"))
    {
        _levelLoader.save(_currentLevel, _em->getEntities());
    }

    ImGui::End();
}
