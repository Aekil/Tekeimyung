/**
* @Author   Mathieu Chassara
*/

#include <Engine/Utils/Logger.hpp>
#include <Engine/Window/HandleFullscreenEvent.hpp>
#include <Engine/Window/GameWindow.hpp>

HandleFullscreenEvent::HandleFullscreenEvent() {}

void    HandleFullscreenEvent::execute()
{
    GameWindow::getInstance()->toggleFullscreen();
}