/**
* @Author   Guillaume Labey
*/

#include <imgui.h>

#include <Engine/Debug/LogDebugWindow.hpp>
#include <Engine/Utils/Helper.hpp>

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

    auto& logLevels = EnumManager<Logger::eLogLevel>::enumStrings;
    uint32_t logLevelsNb = EnumManager<Logger::eLogLevel>::enumLength;
    for (int index = 0; index < logLevelsNb; ++index)
    {
        ImColor     logColorActive;
        ImColor     logColorHover;

        if (index > 5)
        {
            LOG_ERROR("No color for log filter %d", index);
            continue;
        }
        logColorActive = (this->_logColors != nullptr ? this->_logColors[index] : ImGui::GetStyle().Colors[ImGuiCol_ButtonActive]);
        logColorHover = logColorActive;
        //ImGui::PushStyleColor(ImGuiCol_Button, ImColor::HSV(0.0f, 0.1f, 0.5f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_Button, logColorActive);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, logColorHover);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, logColorActive);
        ImGui::Button(logLevels[index]);
        ImGui::PopStyleColor(3);
        if (index < 4) ImGui::SameLine();
    }

    ImGui::SameLine();
    filter.Draw();

    ImGui::Separator();

    ImGui::BeginChild("Logs");
    ImGui::PushTextWrapPos(0.0f);

    // Draw components
    auto& log = _logger->getConsoleLog();
    const char* bufBegin = log.buf.begin();
    const char* line = bufBegin;
    for (int lineNb = 0; line != NULL; lineNb++)
    {
        const char* lineEnd = (lineNb < log.lineOffsets.Size) ? bufBegin + log.lineOffsets[lineNb] : NULL;
        ImGui::TextUnformatted(line, lineEnd);
        line = lineEnd && lineEnd[1] ? lineEnd + 1 : NULL;
    }
    ImGui::PopTextWrapPos();

    // A Log was added since last update
    if ((uint32_t)log.buf.size() > _lastLogSize)
    {
        _lastLogSize = log.buf.size();
        ImGui::SetScrollHere(1.0f);
    }
    ImGui::EndChild();

    ImGui::End();
}
