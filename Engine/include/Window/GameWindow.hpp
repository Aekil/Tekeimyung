#pragma once

#include <memory>
#include <string>
#include <GLFW/glfw3.h>

#define WINDOW_DEFAULT_WIDTH    640
#define WINDOW_DEFAULT_HEIGHT   480
#define WINDOW_DEFAULT_TITLE    "Window default title - Powered by "

class GameWindow
{
public:
    explicit                            GameWindow(int width = WINDOW_DEFAULT_WIDTH, int height = WINDOW_DEFAULT_HEIGHT, const char *title = WINDOW_DEFAULT_TITLE);
    virtual                             ~GameWindow();
    
    int                                 initialize();
    
    int                                 getWidth() const;
    int	                                getHeight() const;
    std::string	                        getTitle() const;
    static std::shared_ptr<GameWindow>  getInstance();
    
    void                                setDecorated(bool decorated);
    void                                setMaximized(bool fullscreen);
    void                                setResizable(bool resizable);
    void                                setInstance(std::shared_ptr<GameWindow> instance);

    bool                                isRunning() const;
    void                                display();
    void                                pollEvents();
    void                                close();
    
private:
    GLFWwindow                          *_window;

    int                                 _screenWidth;
    int                                 _screenHeight;
    int                                 _bufferWidth;
    int                                 _bufferHeight;
    std::string                         _title;

    static std::shared_ptr<GameWindow>  _instance;
};
