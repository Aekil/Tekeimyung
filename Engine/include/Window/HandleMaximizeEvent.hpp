#pragma once

#include    <Window/IInputEvent.hpp>

class   HandleMaximizeEvent : public IInputEvent
{
public:
    explicit    HandleMaximizeEvent();
    virtual     ~HandleMaximizeEvent() {}

    void        execute();
};