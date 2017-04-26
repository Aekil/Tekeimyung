/**
* @Author   Mathieu Chassara
*/

#include <Engine/Debug/Logger.hpp>
#include <Engine/Window/HandleFullscreenEvent.hpp>
#include <Engine/Window/GameWindow.hpp>

HandleFullscreenEvent::HandleFullscreenEvent() {}

void    HandleFullscreenEvent::execute()
{
    GameWindow::getInstance()->toggleFullscreen();
}