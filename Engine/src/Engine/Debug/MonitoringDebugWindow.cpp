/**
* @Author   Julien Chardon
*/

#include <Engine/Debug/Logger.hpp>
#include <Engine/Debug/MonitoringDebugWindow.hpp>


std::shared_ptr<MonitoringDebugWindow>   MonitoringDebugWindow::_monitoringDebugWindow = nullptr;

MonitoringDebugWindow::MonitoringDebugWindow() : DebugWindow("Monitoring"), _checkSec(0) {}

MonitoringDebugWindow::MonitoringDebugWindow(const glm::vec2& pos, const glm::vec2& size) :
    DebugWindow("Monitoring", pos, size), _checkSec(0) {}

MonitoringDebugWindow::~MonitoringDebugWindow() {}

std::shared_ptr<MonitoringDebugWindow>   MonitoringDebugWindow::getInstance()
{
    if (!_monitoringDebugWindow)
    {
        _monitoringDebugWindow = std::make_shared<MonitoringDebugWindow>();
    }
    return (_monitoringDebugWindow);
}

void    MonitoringDebugWindow::build(std::shared_ptr<GameState> gameState, float elapsedTime)
{
    _checkSec += elapsedTime; // update time record

    // Construction of ImGui window with params
    if (!ImGui::Begin(_title.c_str(), &_displayed, ImGuiWindowFlags_NoResize))
    {
        ImGui::End();
        return;
    }

    bool resetCheckSec = false;
    // update & display timeLogs for each monitored system
    for (std::pair<const uint16_t, tMonitoring>& system : _systemsRegistered)
    {
        updateTimeLogsSystem(system.second, &resetCheckSec);
        displaySystem(system.second);
    }
    if (resetCheckSec) // reset time record each 1s past
        _checkSec = 0;

    ImGui::End();
}

void    MonitoringDebugWindow::updateSystem(uint16_t key, float timeSec, uint32_t nbEntities, const char* name)
{
    // Initialize datas
    if (!_systemsRegistered[key].dirty)
    {
        _systemsRegistered[key].dirty = 1;
        _systemsRegistered[key].name = name;
        _systemsRegistered[key].timeSec = 0;
        _systemsRegistered[key].avgTimeSec = 0;
        _systemsRegistered[key].oldAvg = 0;
    }

    _systemsRegistered[key].timeSec = timeSec;
    _systemsRegistered[key].nbEntities = nbEntities;
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

void    MonitoringDebugWindow::updateTimeLogsSystem(tMonitoring& system, bool *resetCheckSec)
{
    if (_checkSec >= 1) // if 1sec past
    {
        // update time average
        system.oldAvg = system.avgTimeSec;
        system.avgTimeSec = calcTimeAverage(system.timeLogs);

        *resetCheckSec = true; // remember to reset time after updating all systems

        system.timeLogs.clear(); // clear timeLogs
    }
    else // if 1sec not past
    {
        system.timeLogs.push_back(system.timeSec); // update timeLogs
    }
}

ImColor    MonitoringDebugWindow::getDisplayColor(tMonitoring& system)
{
    ImColor color;

    // get absolute value of time difference in ms between old and new time average of the system
    float timeDiff = ABS(SEC_TO_MS(system.oldAvg - system.avgTimeSec));

    // define if system display will be colored or not, depending on timeDiff & TIME_DIFF_RATIO
    if (timeDiff > TIME_DIFF_RATIO)
    {
        // process color
        if (system.oldAvg < system.avgTimeSec) // old performance shorter than old one
            color.Value = ImColor(200, 100, 100); // red
        else // new performance shorter than old one
            color.Value = ImColor(100, 200, 100); // green
    }
    else
        color.Value = ImColor(255, 255, 255); // white

    return (color);
}

void    MonitoringDebugWindow::displaySystem(tMonitoring& system)
{
#if (ENABLE_COLOR) // display with colors
    {
        ImColor color = getDisplayColor(system);
        ImGui::TextColored(color, FMT_MSG("%-20s | %4d  : %+3c  %.2f ms   (%.3f)", system.name, (int)system.nbEntities, (system.oldAvg < system.avgTimeSec) ? '+' : '-',
            SEC_TO_MS(system.avgTimeSec), SEC_TO_MS(system.avgTimeSec)).c_str());
    }
#else // display all in white
    {
        ImGui::Text(FMT_MSG("%-20s : %+3c  %.2f ms   (%.3f)", system.name, (system.oldAvg < system.avgTimeSec) ? '+' : '-',
            SEC_TO_MS(system.avgTimeSec), SEC_TO_MS(system.avgTimeSec)).c_str()); // not always uptodate compared to colored display
    }
#endif
    ImGui::Separator(); // separate each display system
}

// old display formating
/*ImGui::Text(FMT_MSG("%-20s : %+2c %.2f ms (%.3f ms)", system.name.c_str(), (system.oldAvg < system.avgTimeSec) ? '+' : '-',
    SEC_TO_MS(system.avgTimeSec), SEC_TO_MS(system.timeSec)).c_str());*/
