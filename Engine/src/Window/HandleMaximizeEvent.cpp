#include <Window/HandleMaximizeEvent.hpp>
#include <Window/GameWindow.hpp>

HandleMaximizeEvent::HandleMaximizeEvent() {}

void    HandleMaximizeEvent::execute()
{
    GameWindow::getInstance()->maximize();
}