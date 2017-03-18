/**
* @Author   Mathieu Chassara
*/

#pragma once

#include    <Engine/Window/IInputEvent.hpp>

class   HandleFullscreenEvent : public IInputEvent
{
public:
    explicit    HandleFullscreenEvent();
    virtual     ~HandleFullscreenEvent() {}

    void        execute();
};