/**
* @Author   Guillaume Labey
*/

#include <iostream>

#include <Engine/Utils/Debug.hpp>

#include <Engine/Utils/DebugWindow.hpp>

DebugWindow::DebugWindow(const std::string& tile, const glm::vec2& pos, const glm::vec2& size):
                        _displayed(ENGINE_DEBUG), _title(tile), _pos(pos), _size(size) {}

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
