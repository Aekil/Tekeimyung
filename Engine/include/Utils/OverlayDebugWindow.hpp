#pragma once

#include "Utils/DebugWindow.hpp"

class OverlayDebugWindow: public DebugWindow
{
public:
    OverlayDebugWindow();
    virtual ~OverlayDebugWindow();

    virtual void        build();
};
