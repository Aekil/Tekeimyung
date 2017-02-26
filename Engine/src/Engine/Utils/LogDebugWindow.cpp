/**
* @Author   Guillaume Labey
*/

#include <imgui.h>

#include <Engine/Utils/LogDebugWindow.hpp>

LogDebugWindow::LogDebugWindow(std::shared_ptr<Logger> logger, const glm::vec2& pos, const glm::vec2& size):
                                DebugWindow("Logs", pos, size), _logger(logger), _lastLogSize(0) {}

LogDebugWindow::~LogDebugWindow() {}

void    LogDebugWindow::build(std::shared_ptr<GameState> gameState, float elapsedTime)
{
    if (!ImGui::Begin(_title.c_str(), &_displayed, ImGuiWindowFlags_NoResize))
    {
        ImGui::End();
        return;
    }
    // Set Window params
    ImGui::SetWindowSize(ImVec2(_size.x, _size.y), ImGuiSetCond_Always);
    ImGui::SetWindowPos(ImVec2(_pos.x, _pos.y), ImGuiSetCond_Always);

    // Draw components
    ImGui::TextUnformatted(_logger->getLog().begin(), _logger->getLog().end());

    if (ImGui::GetItemRectSize().x > _size.x)
    {
        _size.x = ImGui::GetItemRectSize().x;
        if (_size.x > LIMIT_WIDTH)
            _size.x = LIMIT_WIDTH;
    }

    // A Log was added since last update
    if ((uint32_t)_logger->getLog().size() > _lastLogSize)
    {
        _lastLogSize = _logger->getLog().size();
        ImGui::SetScrollHere(1.0f);
    }

    ImGui::End();
}
