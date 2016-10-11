#include <iostream>
#include <GL/glew.h>

#include <Utils/Debug.hpp>
#include <Window/GameWindow.hpp>

std::shared_ptr<GameWindow> GameWindow::_instance;

GameWindow::GameWindow(int width, int height, const char *title) :
    _screenWidth(width), _screenHeight(height),
    _bufferWidth(0), _bufferHeight(0),
    _title(title), _window(nullptr) {}

GameWindow::~GameWindow() {}

bool    GameWindow::initialize()
{
    const GLFWvidmode *vidmode;
    int xPos, yPos;

    // Initializing GLFW.
    if (glfwInit() == GLFW_FALSE)
    {
        std::cerr << "Could not initialize GLFW." << std::endl;
        return (false);
    }


    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Creating a GLFW window.
    _window = glfwCreateWindow(_screenWidth, _screenHeight, _title.c_str(), nullptr, nullptr);

    if (_window == nullptr)
    {
        std::cerr << "Could not initialize the window properly." << std::endl;
        glfwTerminate();
        return (false);
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
        return (false);
    }

    glViewport(0, 0, _bufferWidth, _bufferHeight);

    ImGui_ImplGlfwGL3_Init(_window, true);

    registerEvents();
    return (true);
}

void	GameWindow::registerEvents()
{
	glfwSetWindowUserPointer(_window, this);
	glfwSetKeyCallback(_window, GameWindow::keyCallback);
    glfwSetMouseButtonCallback(_window, GameWindow::buttonCallback);
    glfwSetCursorEnterCallback(_window, GameWindow::cursorEnterCallback);
    glfwSetCursorPosCallback(_window, GameWindow::cursorPositionCallback);
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

Mouse&      GameWindow::getMouse()
{
    return (_mouse);
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
    ImGui_ImplGlfwGL3_Shutdown();
    glfwDestroyWindow(_window);
    glfwTerminate();
}

void	GameWindow::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	GameWindow*     gameWindow;
	Keyboard::eKey  keyboardKey;

    ASSERT(!(key < (int) Keyboard::eKey::UNKNOWN || key >= (int) Keyboard::eKey::LAST), "The keyboard key should belong to the range.");
	gameWindow = reinterpret_cast<GameWindow*>(glfwGetWindowUserPointer(window));
    ASSERT(gameWindow != nullptr, "GameWindow should not be null.");

    Keyboard    &keyboard = gameWindow->getKeyboard();
    keyboardKey = keyboard.getNativeMap()[key];
	switch (action) {
        case GLFW_PRESS:
            {
                if (keyboard.getStateMap()[keyboardKey] == Keyboard::eKeyState::KEY_IDLE)
                   keyboard.getStateMap()[keyboardKey] = Keyboard::eKeyState::KEY_PRESSED;
                else if (keyboard.getStateMap()[keyboardKey] == Keyboard::eKeyState::KEY_PRESSED)
                   keyboard.getStateMap()[keyboardKey] = Keyboard::eKeyState::KEY_MAINTAINED;
            }
            break;
        case GLFW_REPEAT:
            keyboard.getStateMap()[keyboardKey] = Keyboard::eKeyState::KEY_MAINTAINED;
            break;
        case GLFW_RELEASE:
            keyboard.getStateMap()[keyboardKey] = Keyboard::eKeyState::KEY_RELEASED;
            break;
	}
}

void    GameWindow::buttonCallback(GLFWwindow* window, int button, int action, int mods)
{
    GameWindow*     gameWindow;
    Mouse::eButton  mouseButton;

    ASSERT(!(button <= (int) Mouse::eButton::UNKNOWN || button >= (int) Mouse::eButton::MOUSE_BUTTON_LAST), "The mouse button should belong to the range.");
    gameWindow = reinterpret_cast<GameWindow*>(glfwGetWindowUserPointer(window));
    ASSERT(gameWindow != nullptr, "GameWindow should not be null.");

    Mouse       &mouse = gameWindow->getMouse();
    mouseButton = mouse.getNativeMap()[button];
    switch (action)
    {
        case GLFW_PRESS:
            {
                if (mouse.getStateMap()[mouseButton] == Mouse::eButtonState::CLICK_PRESSED)
                    mouse.getStateMap()[mouseButton] = Mouse::eButtonState::CLICK_MAINTAINED;
                else if (mouse.getStateMap()[mouseButton] == Mouse::eButtonState::CLICK_IDLE)
                    mouse.getStateMap()[mouseButton] = Mouse::eButtonState::CLICK_PRESSED;
            }
            break;
        case GLFW_REPEAT:
            mouse.getStateMap()[mouseButton] = Mouse::eButtonState::CLICK_MAINTAINED;
            break;
        case GLFW_RELEASE:
            mouse.getStateMap()[mouseButton] = Mouse::eButtonState::CLICK_RELEASED;
            break;
    }
}

void    GameWindow::cursorEnterCallback(GLFWwindow* window, int entered)
{
    GameWindow*     gameWindow;

    gameWindow = reinterpret_cast<GameWindow*>(glfwGetWindowUserPointer(window));
    ASSERT(gameWindow != nullptr, "GameWindow should not be null.");

    Cursor&     cursor = gameWindow->getMouse().getCursor();
    cursor.setWindowEntering(entered);
}

void    GameWindow::cursorPositionCallback(GLFWwindow* window, double xPos, double yPos)
{
    GameWindow*     gameWindow;

    gameWindow = reinterpret_cast<GameWindow*>(glfwGetWindowUserPointer(window));
    ASSERT(gameWindow != nullptr, "GameWindow should not be null.");

    Cursor&     cursor = gameWindow->getMouse().getCursor();
    cursor.setXPosition(xPos);
    cursor.setYPosition(yPos);
}
