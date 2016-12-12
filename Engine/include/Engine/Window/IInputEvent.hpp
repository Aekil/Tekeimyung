/**
* @Author   Mathieu Chassara
*/

#pragma once

class   IInputEvent
{
public:
    virtual         ~IInputEvent() {}
    virtual void    execute() = 0;
};