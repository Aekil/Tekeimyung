/**
* @Author   Mathieu Chassara
*/

#include    <Engine/Window/Monitor.hpp>

Monitor::Monitor(GLFWmonitor* monitor) : _glfwMonitor(monitor) {}

Monitor::~Monitor() {}

void    Monitor::retrieveVideoModes()
{
    this->_videoModes = glfwGetVideoModes(this->_glfwMonitor, &this->_videoModesCount);
}

void    Monitor::retrievePosition()
{
    glfwGetMonitorPos(this->_glfwMonitor, &this->_xPos, &this->_yPos);
}

void    Monitor::retrievePhysicalSize()
{
    glfwGetMonitorPhysicalSize(this->_glfwMonitor, &this->_widthMM, &this->_heightMM);
}

void    Monitor::retrieveName()
{
    glfwGetMonitorName(this->_glfwMonitor);
}

void    Monitor::retrieveGammaRamp()
{
    this->_gammaramp = glfwGetGammaRamp(this->_glfwMonitor);
}