/**
* @Author   Guillaume Labey
*/

#include <imgui.h>

#include <Engine/Debug/Debug.hpp>
#include <Engine/Debug/DebugWindow.hpp>

DebugWindow::DebugWindow(const std::string& tile, const ImVec2& pos, const ImVec2& size) :
                        _displayed(true), _title(tile), _pos(pos), _size(size) {}

DebugWindow::~DebugWindow() {}

void    DebugWindow::applyGlobalStyle()
{
    ImGuiStyle& style = ImGui::GetStyle();

    style.WindowRounding = 0.0f;
    style.FramePadding = ImVec2(style.FramePadding.x, 4);
    style.ScrollbarRounding = 0.0f;
    style.ScrollbarSize = 20;
}

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

void    DebugWindow::isDisplayed(bool displayed)
{
    _displayed = displayed;
}

void    DebugWindow::setDisableButtonStyle()
{
    // Grey style unselected
    ImGui::PushStyleColor(ImGuiCol_Button, ImColor(0.0f, 0.1f, 0.5f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor(0.75f, 0.75f, 0.75f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor(0.75f, 0.75f, 0.75f, 1.0f));
}

void    DebugWindow::removeDisableButtonStyle()
{
    ImGui::PopStyleColor(3);
}
