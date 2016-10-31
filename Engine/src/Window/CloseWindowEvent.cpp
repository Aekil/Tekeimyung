#include <Window/CloseWindowEvent.hpp>
#include <Window/GameWindow.hpp>

CloseWindowEvent::CloseWindowEvent() {}

void    CloseWindowEvent::execute()
{
    GameWindow::getInstance()->close();
}