/**
* @Author   Guillaume Labey
*/

#include <imgui.h>

#include <Engine/EditorState.hpp>
#include <Engine/EntityFactory.hpp>
#include <Engine/EditorMenuDebugWindow.hpp>
#include <Engine/Utils/LevelLoader.hpp>


EditorMenuDebugWindow::EditorMenuDebugWindow(GameStateManager* gameStateManager, EntityManager* em, const glm::vec2& pos, const glm::vec2& size):
                                    _em(em), _gameStateManager(gameStateManager), DebugWindow("", pos, size) {}

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
    auto& loadedStates = LevelLoader::getInstance()->getLoadedStates();

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
                bool stateRegistered = false;
                for (auto& state: loadedStates)
                {
                    if (state->getLevelFile() == _currentLevel &&
                        _currentLevel.size() > 0)
                    {
                        stateRegistered = true;
                        std::shared_ptr<GameState> gameState = state->create(_gameStateManager);
                        _gameStateManager->addState(gameState);
                        break;
                    }
                }

                if (!stateRegistered && _currentLevel.size() > 0)
                {
                    std::shared_ptr<GameState> gameState = std::make_shared<BasicState>(_gameStateManager);
                    gameState->setLevelFile(_currentLevel);
                    _gameStateManager->addState(gameState);
                }
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
                    _gameStateManager->removeBackState();
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
