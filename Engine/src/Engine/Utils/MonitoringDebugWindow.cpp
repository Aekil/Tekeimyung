#include <imgui.h>

#include <Engine/Utils/MonitoringDebugWindow.hpp>


std::shared_ptr<MonitoringDebugWindow>   MonitoringDebugWindow::_monitoringDebugWindow = nullptr;

//MonitoringDebugWindow::MonitoringDebugWindow() {}

MonitoringDebugWindow::MonitoringDebugWindow(const glm::vec2& pos, const glm::vec2& size) :
    DebugWindow("Monitoring", pos, size)
{
    //_msgList.push_back(FMT_MSG("Collision system : %f secondes", 0.0012564));
    //this->setDisplayed(true);
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

void    MonitoringDebugWindow::build()
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
    /*ImGui::TextUnformatted(_logger->getLog().begin(), _logger->getLog().end());

    if (ImGui::GetItemRectSize().x > _size.x)
        _size.x = ImGui::GetItemRectSize().x;

    // A Log was added since last update
    if ((uint32_t)_logger->getLog().size() > _lastLogSize)
    {
        _lastLogSize = _logger->getLog().size();
        ImGui::SetScrollHere(1.0f);
    }*/

    for (auto&& system : _systemsRegistered)
    {
        
        ImGui::Text(FMT_MSG("%s : %f ms", system.second.name.c_str(), system.second.timeMs).c_str);
        ImGui::Separator();
    }
    //_systemsRegistered.clear();

    ImGui::End();
}

uint16_t    MonitoringDebugWindow::registerSystem(std::string name)
{
    static uint16_t keyID = 0;

    _systemsRegistered[keyID].name = name;
    _systemsRegistered[keyID].timeMs = 0; // ?
    
    return (keyID++);
}

void    MonitoringDebugWindow::updateSystem(uint16_t key, tMonitoring newData)
{
    _systemsRegistered[key].timeMs = newData.timeMs;
}