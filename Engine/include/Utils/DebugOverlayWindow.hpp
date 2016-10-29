#pragma once

#include "Utils/DebugWindow.hpp"

class           DebugOverlayWindow: public DebugWindow
{
public:
    DebugOverlayWindow();
    virtual ~DebugOverlayWindow();

    virtual void        build();
};
