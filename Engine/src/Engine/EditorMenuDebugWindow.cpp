/**
* @Author   Guillaume Labey
*/

#include <imgui.h>

#include <Engine/EditorState.hpp>
#include <Engine/EntityFactory.hpp>
#include <Engine/EditorMenuDebugWindow.hpp>
#include <Engine/Utils/LevelLoader.hpp>


EditorMenuDebugWindow::EditorMenuDebugWindow(GameStateManager* gameStateManager, EntityManager* em, const glm::vec2& pos, const glm::vec2& size):
                                    _em(em), _gameStateManager(gameStateManager), DebugWindow("Editor Menu", pos, size) {}

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

    if (ImGui::BeginMenuBar())
    {
        displayLevelsMenu();
        displayPlayStopMenu();
        ImGui::EndMenuBar();
    }


    ImGui::End();
}

void    EditorMenuDebugWindow::displayLevelsMenu()
{
    bool loadLevel = false;
    bool saveLevel = false;
    bool saveLevelAs = false;
    auto& currentState = _gameStateManager->getCurrentState();

    // Enable menu if we are in play mode, we can't load or save the level
    if (ImGui::BeginMenu("Levels", currentState->getId() == EditorState::identifier))
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
        ImGui::Separator();
        if (ImGui::MenuItem("Save"))
        {
            saveLevel = true;
        }
        else if (ImGui::MenuItem("Save as"))
        {
            saveLevelAs = true;
        }
        ImGui::EndMenu();
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

}

void    EditorMenuDebugWindow::displayPlayStopMenu()
{
    auto& currentState = _gameStateManager->getCurrentState();

    // EditorState should be the first state
    if (_gameStateManager->getStates()[0]->getId() != EditorState::identifier)
    {
        return;
    }

    if (ImGui::BeginMenu("Game"))
    {
        // Play current level
        if (currentState->getId() == EditorState::identifier)
        {
            if (ImGui::MenuItem("Play"))
            {
                std::shared_ptr<GameState> gameState = LevelLoader::getInstance()->createLevelState(_currentLevel, _gameStateManager);
                // Remove the level file because the gameState will copy the EntityManager instead of loading the level
                gameState->setLevelFile("");
                _gameStateManager->addState(gameState, _em);
            }
        }
        // Stop current level
        else
        {
            if (ImGui::MenuItem("Stop"))
            {
                // Remove all states except from the first one (EditorState)
                uint32_t statesNb = (uint32_t)_gameStateManager->getStates().size();
                for (statesNb; statesNb > 1; --statesNb)
                {
                    _gameStateManager->getStates().pop_back();
                }
            }
        }
        ImGui::EndMenu();
    }
}

void    EditorMenuDebugWindow::displayStateNotRegistered()
{
}

void    EditorMenuDebugWindow::displaySaveAsPopup()
{
    static char levelName[64] = "";
    ImGui::InputText("Level name", levelName, 64);
    if (ImGui::Button("Save"))
    {
        _currentLevel = levelName;
        LevelLoader::getInstance()->save(_currentLevel, _em->getEntities());
        LevelLoader::getInstance()->addLevel(_currentLevel);
        ImGui::CloseCurrentPopup();
    }
}

void    EditorMenuDebugWindow::displayLoadPopup()
{
    ImGui::BeginChild("Load level", ImVec2(150, 100), true);
    for (const auto& level: LevelLoader::getInstance()->getLevels())
    {
        if (ImGui::Selectable(level, _tmpLoadLevel == level))
        {
            _tmpLoadLevel = level;
        }
    }
    ImGui::EndChild();

    // No level selected
    if (_tmpLoadLevel.size() == 0)
    {
        setDisableButtonStyle();
        ImGui::Button("Load");
        removeDisableButtonStyle();
    }
    else if (ImGui::Button("Load"))
    {
        _em->destroyAllEntities();
        _currentLevel = _tmpLoadLevel;
        LevelLoader::getInstance()->load(_currentLevel, _em);
        ImGui::CloseCurrentPopup();
    }
}
