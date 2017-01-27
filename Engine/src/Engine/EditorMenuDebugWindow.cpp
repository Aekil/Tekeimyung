/**
* @Author   Guillaume Labey
*/

#include <imgui.h>

#include <Engine/EntityFactory.hpp>
#include <Engine/EditorMenuDebugWindow.hpp>
#include <Engine/Utils/LevelLoader.hpp>


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

    bool loadLevel = false;
    bool saveLevel = false;
    bool saveLevelAs = false;

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Levels"))
        {
            if (ImGui::MenuItem("New"))
            {
                _tmpLoadLevel = "";
                _currentLevel = "";
                _em->destroyAllEntities();
            }
            if (ImGui::MenuItem("Load"))
            {
                _tmpLoadLevel = "";
                loadLevel = true;
            }
            else if (ImGui::MenuItem("Save"))
            {
                saveLevel = true;
            }
            else if (ImGui::MenuItem("Save as"))
            {
                saveLevelAs = true;
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    if (loadLevel)
    {
        ImGui::OpenPopup("Load level");
    }
    else if (saveLevelAs || (saveLevel && _currentLevel.size() == 0))
    {
        ImGui::OpenPopup("Save level as...");
    }
    else if (saveLevel)
    {
        LevelLoader::getInstance()->save(_currentLevel, _em->getEntities());
    }

    if (ImGui::BeginPopup("Save level as..."))
    {
        displaySaveAsPopup();
        ImGui::EndPopup();
    }
    else if (ImGui::BeginPopup("Load level"))
    {
        displayLoadPopup();
        ImGui::EndPopup();
    }

    ImGui::End();
}

void    EditorMenuDebugWindow::displaySaveAsPopup()
{
    static char levelName[64] = "";
    ImGui::InputText("Level name", levelName, 64);
    if (ImGui::Button("Save"))
    {
        _currentLevel = levelName;
        LevelLoader::getInstance()->save(_currentLevel, _em->getEntities());
        LevelLoader::getInstance()->getLevels().push_back(_currentLevel);
        ImGui::CloseCurrentPopup();
    }
}

void    EditorMenuDebugWindow::displayLoadPopup()
{
    ImGui::BeginChild("Load level", ImVec2(150, 100), true);
    for (const auto& level: LevelLoader::getInstance()->getLevels())
    {
        if (ImGui::Selectable(level.c_str(), _tmpLoadLevel == level))
        {
            _tmpLoadLevel = level;
        }
    }
    ImGui::EndChild();

    // No level selected
    if (_tmpLoadLevel.size() == 0)
    {
        // Grey style unselected
        ImGui::PushStyleColor(ImGuiCol_Button, ImColor(0.75f, 0.75f, 0.75f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor(0.75f, 0.75f, 0.75f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor(0.75f, 0.75f, 0.75f, 1.0f));

        ImGui::Button("Load");
        ImGui::PopStyleColor(3);
    }
    else if (ImGui::Button("Load"))
    {
        _em->destroyAllEntities();
        _currentLevel = _tmpLoadLevel;
        LevelLoader::getInstance()->load(_currentLevel, _em);
        ImGui::CloseCurrentPopup();
    }
}
