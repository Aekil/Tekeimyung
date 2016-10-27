#pragma once

#include <string>

class			DebugOverlayWindow
{
public:
	explicit	DebugOverlayWindow();
	virtual		~DebugOverlayWindow() {}

	void		build();

	const std::string	&getTitle() const;
	void				setTitle(const std::string &title);

	bool		isDisplayed() const;
	void		setDisplayed(bool displayed);
private:
	std::string	_title;
	bool		_displayed;
};