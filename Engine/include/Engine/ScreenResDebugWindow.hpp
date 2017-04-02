/**
* @Author   Mathieu Chassara
*/

#pragma once

#include    <GLFW/glfw3.h>

#include    <Engine/Utils/DebugWindow.hpp>
#include    <Engine/Window/VideoMode.hpp>

class   ScreenResDebugWindow : public DebugWindow
{
public:
    ScreenResDebugWindow();
    virtual ~ScreenResDebugWindow();

    void    build(std::shared_ptr<GameState> gameState, float elapsedTime) override final;

    GENERATE_ID(ScreenResDebugWindow);

private:
    void    retrieveGLFWVideoModes();

private:
    GLFWwindow*         _glfwWindow;
    const GLFWvidmode*  _glfwVideoModes;
    int                 _videoModesCount;

    std::vector<VideoMode>      _videoModes;
    std::vector<const char*>    _comboItems;
};