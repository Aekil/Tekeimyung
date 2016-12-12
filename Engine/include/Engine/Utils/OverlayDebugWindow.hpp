/**
* @Author   Mathieu Chassara
*/

#pragma once

#include <Engine/Utils/DebugWindow.hpp>

class OverlayDebugWindow: public DebugWindow
{
public:
    OverlayDebugWindow();
    virtual ~OverlayDebugWindow();

    virtual void        build(float elapsedTime);
};
