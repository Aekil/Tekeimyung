/**
* @Author   Guillaume Labey
*/

#include <imgui.h>

#include <Engine/Utils/Debug.hpp>
#include <Engine/Utils/DebugWindow.hpp>

DebugWindow::DebugWindow(const std::string& tile, const glm::vec2& pos, const glm::vec2& size):
                        _displayed(true), _title(tile), _pos(pos), _size(size) {}

DebugWindow::~DebugWindow() {}

const std::string&  DebugWindow::getTitle() const
{
    return (_title);
}

void    DebugWindow::setTitle(const std::string& title)
{
    _title = title;
}

bool    DebugWindow::isDisplayed() const
{
    return (_displayed);
}

void    DebugWindow::setDisplayed(bool displayed)
{
    _displayed = displayed;
}

void    DebugWindow::setDisableButtonStyle()
{
    // Grey style unselected
    ImGui::PushStyleColor(ImGuiCol_Button, ImColor(0.75f, 0.75f, 0.75f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor(0.75f, 0.75f, 0.75f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor(0.75f, 0.75f, 0.75f, 1.0f));
}

void    DebugWindow::removeDisableButtonStyle()
{
    ImGui::PopStyleColor(3);
}
