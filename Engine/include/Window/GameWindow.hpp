#pragma once

#include <string>
#include <GLFW/glfw3.h>

#define WINDOW_DEFAULT_WIDTH    640
#define WINDOW_DEFAULT_HEIGHT   480
#define WINDOW_DEFAULT_TITLE    "Window default title - Powered by "

class GameWindow
{
public:
    explicit    GameWindow(int width = WINDOW_DEFAULT_WIDTH, int height = WINDOW_DEFAULT_HEIGHT, const char *title = WINDOW_DEFAULT_TITLE);
<<<<<<< HEAD
    virtual	~GameWindow();
    
    int         initialize();
    
    int         width() const;
    int	        height() const;
    std::string	name() const;
    
    void        setDecorated(bool decorated);
    void        setMaximized(bool fullscreen);
    void        setResizable(bool resizable);
    
=======
                ~GameWindow();

    int         initialize();

    int         width() const;
    int         height() const;
    std::string name() const;

    void        setDecorated(bool decorated);
    void        setMaximized(bool fullscreen);
    void        setResizable(bool resizable);

>>>>>>> 283ac34c1adff2944fe0e528fd81d98014c88b39
    bool        isRunning() const;
    void        display();
    void        pollEvents();
    void        close();
<<<<<<< HEAD
    
=======

>>>>>>> 283ac34c1adff2944fe0e528fd81d98014c88b39
private:
    GLFWwindow  *_window;

    int         _screenWidth;
    int         _screenHeight;
    int         _bufferWidth;
    int         _bufferHeight;
    std::string _title;
};
