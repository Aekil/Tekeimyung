/**
* @Author   Mathieu Chassara
*/

#pragma once

#include <Engine/Utils/DebugWindow.hpp>

class OverlayDebugWindow: public DebugWindow
{
public:
    OverlayDebugWindow(const glm::vec2& pos, const glm::vec2& size);
    virtual ~OverlayDebugWindow();

    void            build(std::shared_ptr<GameState> gameState, float elapsedTime) override final;

    GENERATE_ID(OverlayDebugWindow);
};
