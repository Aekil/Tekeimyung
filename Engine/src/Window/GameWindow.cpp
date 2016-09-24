#include	<iostream>

/**
	Please, include the extension loader library before GLFW3.
	See : http://www.glfw.org/docs/latest/quick.html#quick_include
*/
#include	<GL/glew.h>

#include	<GameWindow.hpp>

GameWindow::GameWindow(int width, int height, const char *title) :
	_screenWidth(width), _screenHeight(height),
	_bufferWidth(0), _bufferHeight(0),
	_title(title), _window(nullptr) {}

GameWindow::~GameWindow()
{
	if (this->_window != nullptr)
	{
		//delete (this->_window);
		// deleting the pointer causes an error ????
	}
}

int						GameWindow::initialize()
{
	const GLFWvidmode	*vidmode;
	int					xPos, yPos;

	// Initializing GLFW.
	if (glfwInit() == GLFW_FALSE)
	{
		std::cerr << "Could not initialize GLFW." << std::endl;
		return (1);
	}

	// Creating a GLFW window.
	this->_window = glfwCreateWindow(this->_screenWidth, this->_screenHeight, this->_title.c_str(), nullptr, nullptr);
	if (this->_window == nullptr)
	{
		std::cerr << "Could not initialize the window properly." << std::endl;
		glfwTerminate();
		return (1);
	}
	glfwMakeContextCurrent(this->_window);
	glfwGetFramebufferSize(this->_window, &this->_bufferWidth, &this->_bufferHeight);
	glfwSwapInterval(1);

	// Placing the game window at the center of the screen.
	vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	xPos = (vidmode->width - 640) / 2;
	yPos = (vidmode->height - 480) / 2;
	glfwSetWindowPos(this->_window, xPos, yPos);

	return (0);
}

int			GameWindow::width() const
{
	return (this->_screenWidth);
}

int			GameWindow::height() const
{
	return (this->_screenHeight);
}

std::string	GameWindow::name() const
{
	return (this->_title);
}

void		GameWindow::setDecorated(bool decorated)
{
	glfwWindowHint(GLFW_DECORATED, decorated ? GLFW_TRUE : GLFW_FALSE);
}

void		GameWindow::setMaximized(bool maximized)
{
	glfwWindowHint(GLFW_MAXIMIZED, maximized ? GLFW_TRUE : GLFW_FALSE);
}

void		GameWindow::setResizable(bool resizable)
{
	glfwWindowHint(GLFW_RESIZABLE, resizable ? GLFW_TRUE : GLFW_FALSE);
}

bool		GameWindow::isRunning() const
{
	return (glfwWindowShouldClose(this->_window) == GLFW_FALSE);
}

void		GameWindow::display()
{
	glViewport(0, 0, this->_bufferWidth, this->_bufferHeight);
	glClear(GL_COLOR_BUFFER_BIT);
	glfwSwapBuffers(this->_window);
}

void		GameWindow::pollEvents()
{
	glfwPollEvents();
}

void		GameWindow::close() {
	glfwDestroyWindow(this->_window);
	glfwTerminate();
}