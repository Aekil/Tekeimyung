/**
* @Author   Mathieu Chassara
*/

#pragma once

#include <memory>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/vec4.hpp>

#include <Engine/Core/GameStateManager.hpp>
#include <Engine/Window/Keyboard.hpp>
#include <Engine/Window/Mouse.hpp>
#include <Engine/Utils/Timer.hpp>

#define WINDOW_DEFAULT_TITLE    "Team Tekeimyung - The Squirre"

class GameWindow
{
public:
    explicit                            GameWindow(GameStateManager* gameStateManager, const char *title = WINDOW_DEFAULT_TITLE);
                                        ~GameWindow();

    bool                                initialize();

    int                                 getScreenWidth() const;
    int	                                getScreenHeight() const;
    int                                 getBufferWidth() const;
    int                                 getBufferHeight() const;
    std::string	                        getTitle() const;
    bool                                isFullscreen() const;
    static std::shared_ptr<GameWindow>  getInstance();
	Keyboard&							getKeyboard();
    Mouse&                              getMouse();

    void                                maximize();
    void                                toggleFullscreen();
    void                                setRunning(bool running);
    static void                         setInstance(std::shared_ptr<GameWindow> instance);

    bool                                isCursorVisible() const;
    void                                setCursorVisible(bool visible);

    bool                                isRunning() const;
    void                                display();
    void                                pollEvents();
    void                                close();
    void                                shutdown();

    bool                                hasLostFocus() const;
    void                                hasLostFocus(bool lostFocus);

    Timer&                              getTimer();

    void                                handleResize(int width, int height);

    void                                handleClose(GLFWwindow* window);
    void                                registerCloseHandler(void (*closeHandler)(void*), void* data = nullptr);

    GameStateManager*                   getGameStateManager() const
    {
        return (this->_gameStateManager);
    }
private:
    static void                         closeCallback(GLFWwindow* window);
    static void                         sizeCallback(GLFWwindow* window, int width, int height);
    static void                         focusCallback(GLFWwindow* window, int focused);
    static void                         posCallback(GLFWwindow* window, int xpos, int ypos);

	static void							keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void                         buttonCallback(GLFWwindow* window, int button, int action, int mods);
    static bool                         sendImGuikeyCallback(GameWindow* gameWindow, GLFWwindow* window, int key, int scancode, int action, int mods);
    static bool                         sendImGuibuttonCallback(GameWindow* gameWindow, GLFWwindow* window, int button, int action, int mods);

    static void                         cursorEnterCallback(GLFWwindow* window, int entered);
    static void                         cursorPositionCallback(GLFWwindow* window, double xPos, double yPos);
    static void                         scrollCallback(GLFWwindow* window, double xOffset, double yOffset);

    static void                         charCallback(GLFWwindow* window, unsigned int c);
    static bool                         sendImGuiCharCallback(GameWindow* gameWindow, GLFWwindow* window, unsigned int c);

    void                                registerEvents();

    static void APIENTRY                debugOutput(GLenum source, GLenum type, GLenum id,
                                                    GLenum severity, GLsizei length,
                                                    const GLchar* message, const void* userParam);
    void                                initDebugOutput();
private:
    GLFWmonitor*                        _monitor;
    GLFWwindow*                         _window;
    bool                                _running;

    int                                 _screenWidth;
    int                                 _screenHeight;
    int                                 _bufferWidth;
    int                                 _bufferHeight;

    std::string                         _title;
    bool                                _fullscreen;

    static std::shared_ptr<GameWindow>  _instance;
	Keyboard							_keyboard;
    Mouse                               _mouse;

    GameStateManager*                   _gameStateManager;

    bool                                _lostFocus;
    Timer                               _timer;

    void                                (*_closeHandler)(void*);
    void*                               _closeHandlerData;
};
