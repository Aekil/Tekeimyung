#pragma once

#include		<string>
#include		<GLFW/glfw3.h>

class			GameWindow {
public:
	explicit	GameWindow(int width = 640, int height = 480, const char *title = "Window title");
	~GameWindow();

	int			initialize();

	int			width() const;
	int			height() const;
	std::string	name() const;

	void		setDecorated(bool decorated);
	void		setMaximized(bool fullscreen);
	void		setResizable(bool resizable);

	bool		isRunning() const;
	void		display();
	void		pollEvents();
	void		close();
private:
	GLFWwindow	*_window;

	int			_screenWidth;
	int			_screenHeight;
	int			_bufferWidth;
	int			_bufferHeight;
	std::string	_title;
};
