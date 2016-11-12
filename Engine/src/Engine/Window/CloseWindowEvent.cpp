#include <Engine/Window/CloseWindowEvent.hpp>
#include <Engine/Window/GameWindow.hpp>

CloseWindowEvent::CloseWindowEvent() {}

void    CloseWindowEvent::execute()
{
    GameWindow::getInstance()->close();
}