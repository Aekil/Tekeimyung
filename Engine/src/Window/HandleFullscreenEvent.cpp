#include <Utils/Logger.hpp>
#include <Window/HandleFullscreenEvent.hpp>
#include <Window/GameWindow.hpp>

HandleFullscreenEvent::HandleFullscreenEvent() {}

void    HandleFullscreenEvent::execute()
{
    GameWindow::getInstance()->toggleFullscreen();
}