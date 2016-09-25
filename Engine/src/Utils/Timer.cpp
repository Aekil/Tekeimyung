#include "Utils/Timer.hpp"
#include <GLFW/glfw3.h>

Timer::~Timer() {}

Timer::Timer()
{
    reset();
}

void	Timer::reset()
{
	_lastReset = glfwGetTime();
}

double	Timer::getElapsedTime() const
{
    return glfwGetTime() - _lastReset;
}