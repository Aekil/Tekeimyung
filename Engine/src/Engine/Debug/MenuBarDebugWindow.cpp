/**
* @Author   Guillaume Labey
*/

#include <imgui.h>

#include <Engine/Core/Engine.hpp>
#include <Engine/EditorState.hpp>
#include <Engine/EntityFactory.hpp>
#include <Engine/Debug/MenuBarDebugWindow.hpp>
#include <Engine/Utils/LevelLoader.hpp>

MenuBarDebugWindow::MenuBarDebugWindow(Engine* engine, EntityManager* em) :
    DebugWindow("Editor Menu"), _engine(engine), _em(em) {}

MenuBarDebugWindow::MenuBarDebugWindow(Engine* engine, EntityManager* em, const ImVec2& pos, const ImVec2& size) :
    DebugWindow("Editor Menu", pos, size), _engine(engine), _em(em) {}

MenuBarDebugWindow::~MenuBarDebugWindow() {}

void    MenuBarDebugWindow::build(std::shared_ptr<GameState> gameState, float elapsedTime)
{
    if (!ImGui::Begin(_title.c_str(), &_displayed, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize))
    {
        ImGui::End();
        return;
    }
    
    //  Set Window params
    //ImGui::SetWindowPos(ImVec2(0, 0), ImGuiSetCond_Always);

    EntityManager* em = gameState->getWorld().getEntityManager();

    if (ImGui::BeginMainMenuBar())
    {
        displayLevelsMenu();
        displayPlayStopMenu();
        displayDebugMenu();
        ImGui::EndMainMenuBar();
    }


    ImGui::End();
}

void    MenuBarDebugWindow::displayLevelsMenu()
{
    bool loadLevel = false;
    bool saveLevel = false;
    bool saveLevelAs = false;
    auto& currentState = _engine->getGameStateManager().getCurrentState();

    // Enable menu if we are in play mode, we can't load or save the level
    if (ImGui::BeginMenu("Levels", currentState->getId() == EditorState::identifier))
    {
        if (ImGui::MenuItem("New"))
        {
            newLevel();
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

void    MenuBarDebugWindow::displayPlayStopMenu()
{
    auto& gameStateManager = _engine->getGameStateManager();
    auto& currentState = gameStateManager.getCurrentState();

    // EditorState should be the first state
    if (gameStateManager.getStates()[0]->getId() != EditorState::identifier)
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
                play();
            }
        }
        // Stop current level
        else
        {
            if (ImGui::MenuItem("Stop"))
            {
                // Remove current camera from renderer because it will be deleted when removing states
                Renderer::getInstance()->setCurrentCamera(nullptr);

                // Remove all states except from the first one (EditorState)
                uint32_t statesNb = (uint32_t)gameStateManager.getStates().size();
                for (statesNb; statesNb > 1; --statesNb)
                {
                    gameStateManager.getStates().pop_back();
                }
            }
        }
        ImGui::EndMenu();
    }
}

void    MenuBarDebugWindow::displayDebugMenu()
{
    uint32_t i = 0;
    if (ImGui::BeginMenu("Debug"))
    {
        for (auto& debugWindow: _engine->getDebugWindows())
        {
            // Don't display MenuBarDebugWindow in menu
            if (debugWindow->getId() == MenuBarDebugWindow::identifier)
            {
                continue;
            }

            // Push debug window ID to handle case 2 windows have the same title
            ImGui::PushID(i++);
            bool displayed = debugWindow->isDisplayed();
            if (ImGui::MenuItem(debugWindow->getTitle().c_str(), nullptr, &displayed))
            {
                debugWindow->isDisplayed(displayed);
            }
            ImGui::PopID();
        }

        ImGui::Separator();
        if (ImGui::MenuItem("Display all"))
        {
            _engine->toggleDebugWindowsDisplay(true);
        }
        if (ImGui::MenuItem("Hide all"))
        {
            _engine->toggleDebugWindowsDisplay(false);
        }

        ImGui::EndMenu();
    }
}

void    MenuBarDebugWindow::displaySaveAsPopup()
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

void    MenuBarDebugWindow::displayLoadPopup()
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
        loadLevel(_tmpLoadLevel);
        ImGui::CloseCurrentPopup();
    }
}

void    MenuBarDebugWindow::loadLevel(const std::string& levelName)
{
    _em->destroyAllEntities();
    _currentLevel = levelName;
    _tmpLoadLevel = "";
    LevelLoader::getInstance()->load(_currentLevel, _em);
}

void    MenuBarDebugWindow::play()
{
    auto& gameStateManager = _engine->getGameStateManager();

    std::shared_ptr<GameState> gameState = LevelLoader::getInstance()->createLevelState(_currentLevel, &gameStateManager);
    // Remove the level file because the gameState will copy the EntityManager instead of loading the level
    gameState->setLevelFile("");
    gameStateManager.addState(gameState, _em);
}

void    MenuBarDebugWindow::newLevel()
{
    _tmpLoadLevel = "";
    _currentLevel = "";
    _em->destroyAllEntities();

    EntityFactory::createEntity("CAMERA");
    EntityFactory::createEntity("LIGHT");
}
