/**
* @Author   Guillaume Labey
*/

#include <imgui.h>

#include <Engine/Debug/LogDebugWindow.hpp>

LogDebugWindow::LogDebugWindow(std::shared_ptr<Logger> logger) :
    DebugWindow("Logs"), _logger(logger), _lastLogSize(0)
{
    this->retrieveLogColors();
}

LogDebugWindow::LogDebugWindow(std::shared_ptr<Logger> logger, const ImVec2& pos, const ImVec2& size) :
    DebugWindow("Logs", pos, size), _logger(logger), _lastLogSize(0)
{
    this->retrieveLogColors();
}

LogDebugWindow::~LogDebugWindow()
{
    if (this->_logColors != nullptr)
        delete (this->_logColors);
}

void    LogDebugWindow::retrieveLogColors()
{
    this->_logColors = new ImColor[5];
    this->_logColors[0] = ImColor::HSV(0.3f, 1.0f, 0.5f);
    this->_logColors[1] = ImColor::HSV(0.75f, 0.9f, 0.9f);
    this->_logColors[2] = ImColor::HSV(0.0f, 0.0f, 0.5f);
    this->_logColors[3] = ImColor::HSV(0.06f, 0.9f, 0.9f);
    this->_logColors[4] = ImColor::HSV(0.0f, 0.9f, 0.9f);
}

void    LogDebugWindow::build(std::shared_ptr<GameState> gameState, float elapsedTime)
{
    static ImGuiTextFilter  filter;
    Logger::eLogLevel       level;

    if (!ImGui::Begin(_title.c_str(), &_displayed))
    {
        ImGui::End();
        return;
    }
    
    for (int index = 0; index < 5; ++index)
    {
        ImColor     logColorActive;
        ImColor     logColorHover;

        logColorActive = (this->_logColors != nullptr ? this->_logColors[index] : ImGui::GetStyle().Colors[ImGuiCol_ButtonActive]);
        logColorHover = logColorActive;
        //ImGui::PushStyleColor(ImGuiCol_Button, ImColor::HSV(0.0f, 0.1f, 0.5f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_Button, logColorActive);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, logColorHover);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, logColorActive);
        level = this->_logger->getLevelByIndex(index);
        ImGui::Button(this->_logger->getLevelToString(level).c_str());
        ImGui::PopStyleColor(3);
        if (index < 4) ImGui::SameLine();
    }

    ImGui::SameLine();
    filter.Draw();

    ImGui::Separator();

    ImGui::BeginChild("Logs");

    // Draw components
    ImGui::TextUnformatted(_logger->getLog().begin(), _logger->getLog().end());

    //if (ImGui::GetItemRectSize().x > _size.x)
    //{
    //    _size.x = ImGui::GetItemRectSize().x;
    //    if (_size.x > LIMIT_WIDTH)
    //        _size.x = LIMIT_WIDTH;
    //}

    // A Log was added since last update
    if ((uint32_t)_logger->getLog().size() > _lastLogSize)
    {
        _lastLogSize = _logger->getLog().size();
        ImGui::SetScrollHere(1.0f);
    }
    ImGui::EndChild();

    ImGui::End();
}
