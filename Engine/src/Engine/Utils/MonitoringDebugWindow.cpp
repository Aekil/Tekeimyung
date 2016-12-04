#include <imgui.h>

#include <Engine/Utils/MonitoringDebugWindow.hpp>


std::shared_ptr<MonitoringDebugWindow>   MonitoringDebugWindow::_monitoringDebugWindow = nullptr;

MonitoringDebugWindow::MonitoringDebugWindow(const glm::vec2& pos, const glm::vec2& size) :
    DebugWindow("Monitoring", pos, size)
{
}

MonitoringDebugWindow::~MonitoringDebugWindow() {}

std::shared_ptr<MonitoringDebugWindow>   MonitoringDebugWindow::getInstance()
{
    if (!_monitoringDebugWindow)
    {
        _monitoringDebugWindow = std::make_shared<MonitoringDebugWindow>();
    }
    return _monitoringDebugWindow;
}

void    MonitoringDebugWindow::build(float elapsedTime)
{
    if (!ImGui::Begin(_title.c_str(), &_displayed, ImGuiWindowFlags_NoResize))
    {
        ImGui::End();
        return;
    }
    ImGui::SetWindowSize(ImVec2(_size.x, _size.y), ImGuiSetCond_Always);
    ImGui::SetWindowPos(ImVec2(_pos.x, _pos.y), ImGuiSetCond_Always);

    bool resetTimer = false;
    for (auto&& system : _systemsRegistered)
    {
        if (_checkSec.getElapsedTime() >= 1) // if 1sec past
        {
            // update adverage
            system.second.oldAvg = system.second.avgTimeSec;
            system.second.avgTimeSec = calcTimeAverage(system.second.timeLogs);
            // set bool to reset Timer after the systems loop
            resetTimer = true;
            // clear timeLogs
            system.second.timeLogs.clear();
        }
        else // if 1sec not past
        {
            system.second.timeLogs.push_back(system.second.timeSec); // update timeLogs
        }

        // display monitoring informations (time average)
        if (ENABLE_COLOR) // if macro ENABLE_COLOR = true, display with colors
        {
            // color calculation depending on time variation
            ImColor color;
            bool colored = true;

            if (system.second.oldAvg < system.second.avgTimeSec)
                color.Value = ImColor(200, 100, 100);
            else
                color.Value = ImColor(100, 200, 100);

            float diff = SEC_TO_MS(system.second.oldAvg) - SEC_TO_MS(system.second.avgTimeSec);

            if (diff < 0)
                diff *= -1;

            if (diff < 0.1)
                colored = false;

            if (!colored)
                color.Value = ImColor(255, 255, 255);

            // display informations
            ImGui::TextColored(color, FMT_MSG("%-20s : %+2c %.2f ms (%.3f ms)", system.second.name.c_str(), (system.second.oldAvg < system.second.avgTimeSec) ? '+' : '-',
                SEC_TO_MS(system.second.avgTimeSec), SEC_TO_MS(system.second.timeSec)).c_str());
        }
        else // if not, display in white (default color)
        {
            // display informations
            ImGui::Text(FMT_MSG("%-20s : %+2c %.2f ms (%.3f ms)", system.second.name.c_str(), (system.second.oldAvg < system.second.avgTimeSec) ? '+' : '-',
                SEC_TO_MS(system.second.avgTimeSec), SEC_TO_MS(system.second.timeSec)).c_str());
        }
        ImGui::Separator();
    }
    if (resetTimer) // reset timer if needed (1s past)
        _checkSec.reset();

    ImGui::End();
}

uint16_t    MonitoringDebugWindow::registerSystem(std::string name)
{
    static uint16_t keyID = 0;

    _systemsRegistered[keyID].name = name;
    _systemsRegistered[keyID].timeSec = 0;
    _systemsRegistered[keyID].avgTimeSec = 0;
    _systemsRegistered[keyID].oldAvg = 0;

    return (keyID++);
}

void    MonitoringDebugWindow::updateSystem(uint16_t key, tMonitoring newData)
{
    _systemsRegistered[key].timeSec = newData.timeSec;
}

float   MonitoringDebugWindow::calcTimeAverage(std::vector<float> timeLogs)
{
    float avg = 0;

    for (auto&& time : timeLogs)
    {
        avg += time;
    }
    avg /= timeLogs.capacity();


    return (avg);
}
