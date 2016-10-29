#include <iostream>
#include "Utils/Debug.hpp"

#include "Utils/DebugWindow.hpp"

DebugWindow::DebugWindow(): _displayed(ENGINE_DEBUG) {}

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
