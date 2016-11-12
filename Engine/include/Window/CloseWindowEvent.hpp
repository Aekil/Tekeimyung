#pragma once

#include    <Window/IInputEvent.hpp>

class   CloseWindowEvent : public IInputEvent
{
public:
    explicit    CloseWindowEvent();
    virtual     ~CloseWindowEvent() {}

    void        execute();
};