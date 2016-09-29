#pragma once

#include <memory>
#include <string>
#include <GLFW/glfw3.h>
#include "Window/Keyboard.hpp"

#define WINDOW_DEFAULT_WIDTH    640
#define WINDOW_DEFAULT_HEIGHT   480
#define WINDOW_DEFAULT_TITLE    "Window default title - Powered by "

class GameWindow
{
public:
    explicit                            GameWindow(int width = WINDOW_DEFAULT_WIDTH, int height = WINDOW_DEFAULT_HEIGHT, const char *title = WINDOW_DEFAULT_TITLE);
    virtual                             ~GameWindow();
    
    int                                 initialize();
	void								registerEvents();
    
    int                                 getWidth() const;
    int	                                getHeight() const;
    std::string	                        getTitle() const;
    static std::shared_ptr<GameWindow>  getInstance();
	Keyboard							getKeyboard() const;
    
    void                                setDecorated(bool decorated);
    void                                setMaximized(bool fullscreen);
    void                                setResizable(bool resizable);
    static void                         setInstance(std::shared_ptr<GameWindow> instance);

    bool                                isRunning() const;
    void                                display();
    void                                pollEvents();
    void                                close();

	static void							keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
private:
    GLFWwindow                          *_window;

    int                                 _screenWidth;
    int                                 _screenHeight;
    int                                 _bufferWidth;
    int                                 _bufferHeight;
    std::string                         _title;

    static std::shared_ptr<GameWindow>  _instance;
	Keyboard							_keyboard;
};
