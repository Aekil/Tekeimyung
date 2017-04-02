/**
* @Author   Mathieu Chassara
*/

#pragma once

#include <GLFW/glfw3.h>

class   Monitor
{
public:
    explicit        Monitor(GLFWmonitor* monitor);
    ~Monitor();

public:
    void            retrieveVideoModes();
    void            retrievePosition();
    void            retrievePhysicalSize();
    void            retrieveName();
    void            retrieveGammaRamp();

private:
    GLFWmonitor*    _glfwMonitor;

    const GLFWvidmode*      _videoModes;
    int                     _videoModesCount;
    int                     _xPos;
    int                     _yPos;
    int                     _widthMM;
    int                     _heightMM;
    const char*             _humanReadableName;
    const GLFWgammaramp*    _gammaramp;
};