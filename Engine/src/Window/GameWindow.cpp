#include <iostream>
#include <GL/glew.h>

#include "Window/GameWindow.hpp"

std::shared_ptr<GameWindow> GameWindow::_instance;

GameWindow::GameWindow(int width, int height, const char *title) :
    _screenWidth(width), _screenHeight(height),
    _bufferWidth(0), _bufferHeight(0),
    _title(title), _window(nullptr) {}

GameWindow::~GameWindow() {}

int     GameWindow::initialize()
{
    const GLFWvidmode *vidmode;
    int xPos, yPos;
    
    // Initializing GLFW.
    if (glfwInit() == GLFW_FALSE)
    {
        std::cerr << "Could not initialize GLFW." << std::endl;
        return (1);
    }
    

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // Creating a GLFW window.
    _window = glfwCreateWindow(_screenWidth, _screenHeight, _title.c_str(), nullptr, nullptr);

    if (_window == nullptr)
    {
        std::cerr << "Could not initialize the window properly." << std::endl;
        glfwTerminate();
        return (1);
    }

    glfwMakeContextCurrent(_window);
    glfwGetFramebufferSize(_window, &_bufferWidth, &_bufferHeight);


    // Enabling vertical synchronization (or VSync).
    glfwSwapInterval(1);
    
    // Placing the game window at the center of the screen.
    vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    xPos = (vidmode->width - WINDOW_DEFAULT_WIDTH) / 2;
    yPos = (vidmode->height - WINDOW_DEFAULT_HEIGHT) / 2;
    glfwSetWindowPos(_window, xPos, yPos);


    // Init Glew
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to init glew" << std::endl;
        return (1);
    }

    glViewport(0, 0, _bufferWidth, _bufferHeight);

    registerEvents();
    return (0);
}

void	GameWindow::registerEvents()
{
	glfwSetWindowUserPointer(_window, &_keyboard);
	glfwSetKeyCallback(_window, GameWindow::keyCallback);
}

int     GameWindow::getWidth() const
{
    return (_screenWidth);
}

int     GameWindow::getHeight() const
{
    return (_screenHeight);
}

std::string     GameWindow::getTitle() const
{
    return (_title);
}

std::shared_ptr<GameWindow> GameWindow::getInstance()
{
    return (_instance);
}

Keyboard&	GameWindow::getKeyboard()
{
	return (_keyboard);
}

void    GameWindow::setDecorated(bool decorated)
{
    glfwWindowHint(GLFW_DECORATED, decorated ? GLFW_TRUE : GLFW_FALSE);
}

void    GameWindow::setMaximized(bool maximized)
{
    glfwWindowHint(GLFW_MAXIMIZED, maximized ? GLFW_TRUE : GLFW_FALSE);
}

void    GameWindow::setResizable(bool resizable)
{
    glfwWindowHint(GLFW_RESIZABLE, resizable ? GLFW_TRUE : GLFW_FALSE);
}

void    GameWindow::setInstance(std::shared_ptr<GameWindow> instance)
{
    _instance = instance;
}

bool    GameWindow::isRunning() const
{
    return (glfwWindowShouldClose(_window) == GLFW_FALSE);
}

void    GameWindow::display()
{
    glfwSwapBuffers(_window);
}

void    GameWindow::pollEvents()
{
    glfwPollEvents();
}

void    GameWindow::close()
{
    glfwDestroyWindow(_window);
    glfwTerminate();
}

void	GameWindow::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	Keyboard *keyboard;
	Keyboard::Key keyboardKey;

	keyboard = reinterpret_cast<Keyboard *>(glfwGetWindowUserPointer(window));
	if (keyboard != nullptr) {
		keyboardKey = keyboard->getNativeMap()[key];
		switch (action) {
			case GLFW_PRESS:
				keyboard->getStateMap()[keyboardKey] = Keyboard::KeyState::KEY_PRESSED;
				break;
			case GLFW_REPEAT:
				keyboard->getStateMap()[keyboardKey] = Keyboard::KeyState::KEY_MAINTAINED;
				break;
			case GLFW_RELEASE:
				keyboard->getStateMap()[keyboardKey] = Keyboard::KeyState::KEY_RELEASED;
				break;
		}
	}
}