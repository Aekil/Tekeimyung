/**
* @Author   Mathieu Chassara
*/

#include <imgui.h>

#include <Engine/Debug/OverlayDebugWindow.hpp>
#include <Engine/Window/GameWindow.hpp>

OverlayDebugWindow::OverlayDebugWindow() :
    DebugWindow("Overlay") {}

OverlayDebugWindow::OverlayDebugWindow(const glm::vec2& pos, const glm::vec2& size) :
    DebugWindow("Overlay", pos, size) {}

OverlayDebugWindow::~OverlayDebugWindow() {}

void    OverlayDebugWindow::build(std::shared_ptr<GameState> gameState, float elapsedTime)
{
    if (!ImGui::Begin(_title.c_str(), &_displayed, ImVec2(0, 0), 0.3f,
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize))
    {
        ImGui::End();
        return;
    }


    //ImGui::SetWindowSize(ImVec2(_size.x, _size.y), ImGuiSetCond_Always);

    //ImVec2 windowSize = ImGui::GetWindowSize();
    //_pos.x = GameWindow::getInstance()->getBufferWidth() / 2.0f - windowSize.x / 2.0f;
    //ImGui::SetWindowPos(ImVec2(_pos.x, _pos.y), ImGuiSetCond_Always);

    ImGui::Text("Average: %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::Separator();
    ImGui::Text("Mouse Position: (%.1f,%.1f)", ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y);
    ImGui::End();
}
