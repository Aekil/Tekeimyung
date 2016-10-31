#pragma once

#include    <Window/IInputEvent.hpp>

class   HandleFullscreenEvent : public IInputEvent
{
public:
    explicit    HandleFullscreenEvent();
    virtual     ~HandleFullscreenEvent() {}

    void        execute();
};