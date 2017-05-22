/**
* @Author   Mathieu Chassara
*/

#pragma once

#include    <Engine/Window/IInputEvent.hpp>

class   HandleMaximizeEvent : public IInputEvent
{
public:
    explicit    HandleMaximizeEvent();
    virtual     ~HandleMaximizeEvent() {}

    void        execute();
};