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

    virtual void        build(float elapsedTime);

    GENERATE_ID(OverlayDebugWindow);
};
