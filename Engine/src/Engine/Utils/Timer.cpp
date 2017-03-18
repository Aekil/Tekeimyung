/**
* @Author   Guillaume Labey
*/

#include <GLFW/glfw3.h>

#include <Engine/Utils/Timer.hpp>

Timer::Timer()
{
    reset();
}

Timer::~Timer() {}

void    Timer::reset()
{
    _lastReset = (float)glfwGetTime();
}

float   Timer::getElapsedTime() const
{
    return ((float)glfwGetTime() - _lastReset);
}
