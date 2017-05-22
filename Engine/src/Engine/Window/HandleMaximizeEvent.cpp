/**
* @Author   Mathieu Chassara
*/

#include <Engine/Window/HandleMaximizeEvent.hpp>
#include <Engine/Window/GameWindow.hpp>

HandleMaximizeEvent::HandleMaximizeEvent() {}

void    HandleMaximizeEvent::execute()
{
    GameWindow::getInstance()->maximize();
}