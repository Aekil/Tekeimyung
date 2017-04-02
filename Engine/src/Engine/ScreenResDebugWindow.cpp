/**
* @Author   Mathieu Chassara
*/

#include    <imgui.h>

#include    <Engine/Utils/Logger.hpp>
#include    <Engine/Window/GameWindow.hpp>
#include    <Engine/ScreenResDebugWindow.hpp>

ScreenResDebugWindow::ScreenResDebugWindow() : DebugWindow("Screen Resolution")
{
    this->_glfwWindow = nullptr;
    this->_glfwVideoModes = nullptr;
    this->_videoModesCount = 0;
}

ScreenResDebugWindow::~ScreenResDebugWindow() {}

void    ScreenResDebugWindow::build(std::shared_ptr<GameState> gameState, float elapsedTime)
{
    static bool         done = false;
    static int          current_item = -1;

    if (GameWindow::getInstance()->isFullscreen())
    {
        if (!ImGui::Begin(_title.c_str(), &_displayed, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::End();
            return;
        }

        if (done == false)
        {
            this->retrieveGLFWVideoModes();
            if (this->_videoModesCount > 0)
            {
                const GLFWvidmode*    vidModes = this->_glfwVideoModes;
                const char*     vidModeItem = nullptr;

                for (int index = 0; index < this->_videoModesCount; ++index, ++vidModes)
                {
                    this->_videoModes.push_back(VideoMode(vidModes));
                    vidModeItem = VideoMode::toConstString(this->_videoModes[index]);
                    this->_comboItems.push_back(vidModeItem);
                }
                done = true;
            }
            ImGui::Combo("Screen resolution(s)", &current_item, this->_comboItems.data(), this->_videoModesCount);
        }
        ImGui::End();
    }
    else
        done = false;
}

void    ScreenResDebugWindow::retrieveGLFWVideoModes()
{
    std::shared_ptr<GameWindow> gameWindow = GameWindow::getInstance();

    ASSERT(gameWindow != nullptr, "GameWindow's instance should not be null.");
    if (gameWindow->isFullscreen())
    {
        GLFWmonitor*    monitor;

        this->_glfwWindow = GameWindow::getInstance()->getWindow();
        ASSERT(this->_glfwWindow != nullptr, "GLFWwindow* should not be null and should exist at this moment.");
        monitor = glfwGetWindowMonitor(this->_glfwWindow);
        ASSERT(monitor != nullptr, "GLFWmonitor* is null despite GameWindow being in fullscreen. (when fullscreen, it should not be null !)");
        this->_glfwVideoModes = glfwGetVideoModes(monitor, &this->_videoModesCount);
    }
}