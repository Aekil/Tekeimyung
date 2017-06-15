/**
* @Author   Mathieu Chassara
*/

#include <iostream>
#include <GL/glew.h>
#include <imgui.h>
#include <imgui_impl_glfw_gl3.h>

#include <Engine/Graphics/Renderer.hpp>
#include <Engine/Sound/SoundManager.hpp>
#include <Engine/Debug/Debug.hpp>
#include <Engine/Debug/Logger.hpp>

#include <Engine/Window/GameWindow.hpp>

std::shared_ptr<GameWindow> GameWindow::_instance;

GameWindow::GameWindow(GameStateManager* gameStateManager, const char *title) :
    _bufferWidth(0), _bufferHeight(0), _fullscreen(false),
    _title(title), _monitor(nullptr), _window(nullptr), _running(false), _gameStateManager(gameStateManager),
    _lostFocus(false), _closeHandler(nullptr) {}

GameWindow::~GameWindow() {}

bool    GameWindow::initialize()
{
    GLFWmonitor*        monitor = nullptr;
    const GLFWvidmode*  vidmode = nullptr;
    int width = 0, height = 0;

    // Initializing GLFW.
    if (glfwInit() == GLFW_FALSE)
    {
        LOG_ERROR("Could not initialize GLFW properly.");
        //std::cerr << "Could not initialize GLFW properly." << std::endl;
        return (false);
    }

    // Retrieving the primary monitor.
    _monitor = glfwGetPrimaryMonitor();
    if (_monitor == nullptr)
    {
        LOG_ERROR("Could not retrieve the primary monitor properly.");
        //std::cerr << "Could not retrieve the primary monitor properly." << std::endl;
        glfwTerminate();
        return (false);
    }

    // Retrieving the video mode.
    vidmode = glfwGetVideoMode(_monitor);
    glfwWindowHint(GLFW_RED_BITS, vidmode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, vidmode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, vidmode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, vidmode->refreshRate);
    _screenWidth = vidmode->width;
    _screenHeight = vidmode->height;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Creating a GLFW window.
    if (_fullscreen == true)
        monitor = _monitor;
    else
    {
        glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
        monitor = nullptr;
    }
    _window = glfwCreateWindow(_screenWidth, _screenHeight, _title.c_str(), monitor, nullptr);
    if (_window == nullptr)
    {
        LOG_ERROR("Could not initialize the window properly.");
        //std::cerr << "Could not initialize the window properly." << std::endl;
        glfwTerminate();
        return (false);
    }

    glfwMakeContextCurrent(_window);
    glfwGetFramebufferSize(_window, &_bufferWidth, &_bufferHeight);
    glfwSwapInterval(0);

    // Initializing Glew.
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        LOG_ERROR("Failed to initialize glew properly.");
        //std::cerr << "Failed to initialize glew properly." << std::endl;
        glfwTerminate();
        return (false);
    }

    this->registerEvents();
    ImGui_ImplGlfwGL3_Init(_window, false);

    this->setRunning(true);

    #if defined(ENGINE_DEBUG)
        this->initDebugOutput();
    #endif

    return (true);
}

void	GameWindow::registerEvents()
{
	glfwSetWindowUserPointer(_window, this);
    glfwSetKeyCallback(_window, GameWindow::keyCallback);
    glfwSetCharCallback(_window, GameWindow::charCallback);
    glfwSetMouseButtonCallback(_window, GameWindow::buttonCallback);
    glfwSetCursorEnterCallback(_window, GameWindow::cursorEnterCallback);
    glfwSetCursorPosCallback(_window, GameWindow::cursorPositionCallback);
    glfwSetScrollCallback(_window, GameWindow::scrollCallback);
    glfwSetWindowFocusCallback(_window, GameWindow::focusCallback);
    glfwSetWindowPosCallback(_window, GameWindow::posCallback);
    glfwSetWindowCloseCallback(_window, GameWindow::closeCallback);
    glfwSetWindowSizeCallback(_window, GameWindow::sizeCallback);
}

void APIENTRY   GameWindow::debugOutput(GLenum source, GLenum type, GLenum id,
                            GLenum severity, GLsizei length,
                            const GLchar* message, const void* userParam)
{
    std::string sourceString;
    std::string typeString;

    switch (source)
    {
        case GL_DEBUG_SOURCE_API:
            sourceString = "SOURCE_API";
        break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            sourceString = "WINDOW_SYSTEM";
        break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            sourceString = "SHADER_COMPILER";
        break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:
            sourceString = "THIRD_PARTY";
        break;
        case GL_DEBUG_SOURCE_APPLICATION:
            sourceString = "SOURCE_APPLICATION";
        break;
        case GL_DEBUG_SOURCE_OTHER:
            sourceString = "SOURCE_OTHER";
        break;
        default:
            sourceString = "UNDEFINED";
        break;
    }

    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:
            typeString = "ERROR";
        break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            typeString = "DEPRECATED_BEHAVIOR";
        break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            typeString = "UNDEFINED_BEHAVIOR";
        break;
        case GL_DEBUG_TYPE_PORTABILITY:
            typeString = "PORTABILITY";
        break;
        case GL_DEBUG_TYPE_PERFORMANCE:
            typeString = "PERFORMANCE";
        break;
        case GL_DEBUG_TYPE_MARKER:
            typeString = "MARKER";
        break;
        case GL_DEBUG_TYPE_PUSH_GROUP:
            typeString = "PUSH_GROUP";
        break;
        case GL_DEBUG_TYPE_POP_GROUP:
            typeString = "POP_GROUP";
        break;
        case GL_DEBUG_TYPE_OTHER:
            typeString = "OTHER";
        break;
        default:
            typeString = "UNDEFINED";
        break;
    }

    if (severity == GL_DEBUG_SEVERITY_HIGH)
    {
        LOG_ERROR("[OpenGL][%s][%s] %s", sourceString.c_str(), typeString.c_str(), message);
    }
    else if (severity == GL_DEBUG_SEVERITY_MEDIUM ||
        severity == GL_DEBUG_SEVERITY_LOW)
    {
        LOG_WARN("[OpenGL][%s][%s] %s", sourceString.c_str(), typeString.c_str(), message);
    }
}

void    GameWindow::initDebugOutput()
{
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(GameWindow::debugOutput, nullptr);
}

int     GameWindow::getScreenWidth() const
{
    return (_screenWidth);
}

int     GameWindow::getScreenHeight() const
{
    return (_screenHeight);
}

int     GameWindow::getBufferWidth() const
{
    return (_bufferWidth);
}

int     GameWindow::getBufferHeight() const
{
    return (_bufferHeight);
}

std::string     GameWindow::getTitle() const
{
    return (_title);
}

bool            GameWindow::isFullscreen() const
{
    GLFWmonitor*    monitor = nullptr;

    monitor = glfwGetWindowMonitor(_window);
    return (monitor != nullptr);
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

void    GameWindow::maximize()
{
    glfwMaximizeWindow(_window);
}

void    GameWindow::toggleFullscreen()
{
    const GLFWvidmode*  vidmode = nullptr;
    GLFWmonitor*        monitor = nullptr;
    int                 refreshRate = 0;

    monitor = glfwGetWindowMonitor(_window);
    if (monitor != nullptr)
    {
        vidmode = glfwGetVideoMode(monitor);
        _screenWidth = vidmode->width;
        _screenHeight = vidmode->height;
        refreshRate = vidmode->refreshRate;
        glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
        glfwSetWindowMonitor(_window, nullptr, 0, 0, _screenWidth, _screenHeight, refreshRate);
        glfwGetFramebufferSize(_window, &_bufferWidth, &_bufferHeight);
        handleResize(_bufferWidth, _bufferHeight);
        LOG_INFO("Toggled windowed mode.");
    }
    else
    {
        vidmode = glfwGetVideoMode(_monitor);
        _screenWidth = vidmode->width;
        _screenHeight = vidmode->height;
        refreshRate = vidmode->refreshRate;
        glfwSetWindowMonitor(_window, _monitor, 0, 0, _screenWidth, _screenHeight, refreshRate);
        glfwGetFramebufferSize(_window, &_bufferWidth, &_bufferHeight);
        handleResize(_bufferWidth, _bufferHeight);
        LOG_INFO("Toggled fullscreen mode.");
    }
}

void    GameWindow::setRunning(bool running)
{
    _running = running;
}

void    GameWindow::setInstance(std::shared_ptr<GameWindow> instance)
{
    _instance = instance;
}

bool    GameWindow::isCursorVisible() const
{
    return (glfwGetInputMode(_window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL);
}

void        GameWindow::setCursorVisible(bool visible)
{
    int     visibleValue;

    visibleValue = visible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_HIDDEN;
    glfwSetInputMode(_window, GLFW_CURSOR, visibleValue);
}

bool    GameWindow::isRunning() const
{
    //return (_running);
    return (glfwWindowShouldClose(_window) == GLFW_FALSE);
}

void    GameWindow::display()
{
    glfwSwapBuffers(_window);
}

void    GameWindow::pollEvents()
{
    _keyboard.updateKeyboardState();
    _keyboard.resetTypedText();
    _mouse.updateMouseState();
    glfwPollEvents();
}

void    GameWindow::close()
{
    //glfwSetWindowShouldClose(_window, 0);
    ImGui_ImplGlfwGL3_Shutdown();
    glfwDestroyWindow(_window);
    glfwTerminate();
}

void    GameWindow::shutdown()
{
    ImGui_ImplGlfwGL3_Shutdown();
    glfwDestroyWindow(_window);
    glfwTerminate();
}

/**
    Callback function used to close the GameWindow and other stuff properly.
*/
void    GameWindow::closeCallback(GLFWwindow* window)
{
    GameWindow*     gameWindow;
    gameWindow = reinterpret_cast<GameWindow*>(glfwGetWindowUserPointer(window));
    ASSERT(gameWindow != nullptr, "GameWindow should not be null.");

    gameWindow->handleClose(window);
}

/**
    Callback function used to handle the resize of the window
*/
void    GameWindow::sizeCallback(GLFWwindow* window, int width, int height)
{
    GameWindow*     gameWindow;
    gameWindow = reinterpret_cast<GameWindow*>(glfwGetWindowUserPointer(window));
    ASSERT(gameWindow != nullptr, "GameWindow should not be null.");

    gameWindow->handleResize(width, height);
}


/**
    Callback function used to handle the auto pause of the game
    when the window lose the focus
*/
void    GameWindow::focusCallback(GLFWwindow* window, int focused)
{
    GameWindow*     gameWindow;

    gameWindow = reinterpret_cast<GameWindow*>(glfwGetWindowUserPointer(window));
    ASSERT(gameWindow != nullptr, "GameWindow should not be null.");

    // The window gained the focus
    if (focused)
    {
        SoundManager::getInstance()->resumeAllChannels();
        gameWindow->hasLostFocus(false);
    }
    // The window lost focus
    else
    {
        SoundManager::getInstance()->pauseAllChannels();
        gameWindow->hasLostFocus(true);
    }
}

/**
    Callback function called when the window move
*/
void   GameWindow::posCallback(GLFWwindow* window, int xpos, int ypos)
{
    GameWindow*     gameWindow;

    gameWindow = reinterpret_cast<GameWindow*>(glfwGetWindowUserPointer(window));
    ASSERT(gameWindow != nullptr, "GameWindow should not be null.");

    Timer& timer = gameWindow->getTimer();
    timer.reset();
}

/**
    Callback function used to handle the Keyboard class through keys' states.
    This function only retrieves a specific key and updates its state in the keys' map.
*/
void	GameWindow::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	GameWindow*     gameWindow;
	Keyboard::eKey  keyboardKey;

    gameWindow = static_cast<GameWindow*>(glfwGetWindowUserPointer(window));
    ASSERT(gameWindow != nullptr, "GameWindow should not be null.");

    // Don't capture keyboard events if ImGui is capturing them
    if (sendImGuikeyCallback(gameWindow, window, key, scancode, action, mods))
        return;

    Keyboard&       keyboard = gameWindow->getKeyboard();
    keyboardKey = keyboard.getNativeMap()[key];
    if (keyboardKey <= Keyboard::eKey::UNKNOWN || keyboardKey >= Keyboard::eKey::LAST)
        LOG_INFO("The keyboard key should belong to the range.");
    else
    {
        switch (action)
        {
            case GLFW_PRESS:
                if (keyboard.getStateMap()[keyboardKey] == Keyboard::eKeyState::KEY_IDLE)
                    keyboard.getStateMap()[keyboardKey] = Keyboard::eKeyState::KEY_PRESSED;
                else if (keyboard.getStateMap()[keyboardKey] == Keyboard::eKeyState::KEY_PRESSED)
                    keyboard.getStateMap()[keyboardKey] = Keyboard::eKeyState::KEY_MAINTAINED;
                break;
            case GLFW_REPEAT:
                keyboard.getStateMap()[keyboardKey] = Keyboard::eKeyState::KEY_MAINTAINED;
                break;
            case GLFW_RELEASE:
                keyboard.getStateMap()[keyboardKey] = Keyboard::eKeyState::KEY_RELEASED;
                break;
        }
    }
}

bool    GameWindow::sendImGuikeyCallback(GameWindow* gameWindow, GLFWwindow* window, int key, int scancode, int action, int mods)
{
    ImGuiIO&        io = ImGui::GetIO();
    Keyboard&       keyboard = gameWindow->getKeyboard();
    static bool     keyboardStateReset = false;

    // ImGui is capturing keyboard
    if (io.WantCaptureKeyboard)
    {
        // Use keyboardStateReset to reset keyboard state only one time
        if (!keyboardStateReset)
            keyboard.resetKeyboardState();
        else
            keyboardStateReset = true;
        ImGui_ImplGlfwGL3_KeyCallback(window, key, scancode, action, mods);
        return (true);
    }
    else
        keyboardStateReset = false;

    return (false);
}

/**
    Callback function used to handle the Mouse class through buttons' states.
    This function only retrieve a specific button and updates its state in the buttons' map.
*/
void    GameWindow::buttonCallback(GLFWwindow* window, int button, int action, int mods)
{
    GameWindow*     gameWindow;
    Mouse::eButton  mouseButton;

    gameWindow = static_cast<GameWindow*>(glfwGetWindowUserPointer(window));
    ASSERT(gameWindow != nullptr, "GameWindow should not be null.");

    // Don't capture mouse events if ImGui is capturing them
    if (sendImGuibuttonCallback(gameWindow, window, button, action, mods))
        return;

    Mouse& mouse =  gameWindow->getMouse();
    mouseButton = mouse.getNativeMap()[button];
    if (button <= (int) Mouse::eButton::UNKNOWN || button >= (int) Mouse::eButton::MOUSE_BUTTON_LAST)
        LOG_INFO("The mouse button should belong to the range.");
    else
    {
        switch (action)
        {
            case GLFW_PRESS:
                if (mouse.getStateMap()[mouseButton] == Mouse::eButtonState::CLICK_PRESSED)
                    mouse.getStateMap()[mouseButton] = Mouse::eButtonState::CLICK_MAINTAINED;
                else if (mouse.getStateMap()[mouseButton] == Mouse::eButtonState::CLICK_IDLE)
                    mouse.getStateMap()[mouseButton] = Mouse::eButtonState::CLICK_PRESSED;
                break;
            case GLFW_REPEAT:
                mouse.getStateMap()[mouseButton] = Mouse::eButtonState::CLICK_MAINTAINED;
                break;
            case GLFW_RELEASE:
                mouse.getStateMap()[mouseButton] = Mouse::eButtonState::CLICK_RELEASED;
                break;
        }
    }
}

bool    GameWindow::sendImGuibuttonCallback(GameWindow* gameWindow, GLFWwindow* window, int button, int action, int mods)
{
    ImGuiIO&        io = ImGui::GetIO();
    Mouse&          mouse = gameWindow->getMouse();
    static bool     mouseStateReset = false;

    // ImGui is capturing mouse
    if (io.WantCaptureMouse)
    {
        // Use mouseStateReset to reset mouse state only one time
        if (!mouseStateReset)
            mouse.resetMouseState();
        else
            mouseStateReset = true;
        ImGui_ImplGlfwGL3_MouseButtonCallback(window, button, action, mods);
        return (true);
    }
    else
        mouseStateReset = false;

    return (false);
}

/**
    Callback function used to handle whether the cursor has entered into the GameWindow frame or not.
*/
void    GameWindow::cursorEnterCallback(GLFWwindow* window, int entered)
{
    GameWindow*     gameWindow;

    gameWindow = static_cast<GameWindow*>(glfwGetWindowUserPointer(window));
    ASSERT(gameWindow != nullptr, "GameWindow should not be null.");

    Cursor& cursor = gameWindow->getMouse().getCursor();
    cursor.setWindowEntering(entered == 1);
}

/**
    Callback function used to handle the cursor position into the GameWindow frame.
    This function sets the current x and y positions of the cursor, into the frame.
*/
void    GameWindow::cursorPositionCallback(GLFWwindow* window, double xPos, double yPos)
{
    GameWindow*     gameWindow;

    gameWindow = static_cast<GameWindow*>(glfwGetWindowUserPointer(window));
    ASSERT(gameWindow != nullptr, "GameWindow should not be null.");

    Cursor& cursor = gameWindow->getMouse().getCursor();
    cursor.setXPosition(xPos);
    cursor.setYPosition(yPos);
}

void    GameWindow::scrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
    GameWindow*     gameWindow;

    gameWindow = static_cast<GameWindow*>(glfwGetWindowUserPointer(window));
    ASSERT(gameWindow != nullptr, "GameWindow should not be null.");

    // Don't capture mouse events if ImGui is capturing them
    if (sendImGuiScrollCallback(window, xOffset, yOffset))
        return;

    sScroll&    scroll = gameWindow->getMouse().getScroll();

    scroll.xOffset += xOffset;
    scroll.yOffset += yOffset;
}

bool    GameWindow::sendImGuiScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
    ImGuiIO&    io = ImGui::GetIO();

    if (io.WantCaptureMouse)
    {
        ImGui_ImplGlfwGL3_ScrollCallback(window, xOffset, yOffset);
        return (true);
    }
    return (false);
}

void    GameWindow::charCallback(GLFWwindow* window, unsigned int c)
{
    GameWindow*     gameWindow;

    gameWindow = static_cast<GameWindow*>(glfwGetWindowUserPointer(window));
    ASSERT(gameWindow != nullptr, "GameWindow should not be null.");

    // Don't capture char events if ImGui is capturing them
    if (sendImGuiCharCallback(gameWindow, window, c))
        return;

    gameWindow->getKeyboard().addTypedChar(c);
}

bool    GameWindow::sendImGuiCharCallback(GameWindow* gameWindow, GLFWwindow* window, unsigned int c)
{
    ImGuiIO&    io = ImGui::GetIO();

    // ImGui is capturing keyboard
    if (io.WantCaptureKeyboard)
    {
        ImGui_ImplGlfwGL3_CharCallback(window, c);
        return (true);
    }
    return (false);
}

bool    GameWindow::hasLostFocus() const
{
    return (_lostFocus);
}

void    GameWindow::hasLostFocus(bool lostFocus)
{
    _lostFocus = lostFocus;
}

Timer&  GameWindow::getTimer()
{
    return (_timer);
}

void    GameWindow::handleResize(int width, int height)
{
    // Don't handle resize if lost focus
    // (Because with and height are 0)
    if (_lostFocus)
        return;

    if (width && height)
    {
        _bufferWidth = width;
        _bufferHeight = height;

        // Handle window resize for menu systems
        for (auto& gameState: _gameStateManager->getStates())
        {
            gameState->onWindowResize();
        }

        Renderer::getInstance()->onWindowResize();
    }
}

void    GameWindow::handleClose(GLFWwindow* window)
{
    if (_closeHandler)
    {
        // Window is minimized
        if (_lostFocus)
        {
            glfwRestoreWindow(window);
        }
        glfwSetWindowShouldClose(window, false);
        _closeHandler(_closeHandlerData);
    }
}

void    GameWindow::registerCloseHandler(void (*closeHandler)(void*), void* data)
{
    _closeHandler = closeHandler;
    _closeHandlerData = data;
}
