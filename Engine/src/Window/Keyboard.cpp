#include <iostream>
#include <GLFW/glfw3.h>

#include "Window/Keyboard.hpp"

Keyboard::Keyboard()
{
	_nativeMap[GLFW_KEY_UNKNOWN] = Keyboard::Key::UNKNOWN;
	_nativeMap[GLFW_KEY_W] = Keyboard::Key::Z;
	_nativeMap[GLFW_KEY_A] = Keyboard::Key::Q;
	_nativeMap[GLFW_KEY_S] = Keyboard::Key::S;
	_nativeMap[GLFW_KEY_D] = Keyboard::Key::D;
	_nativeMap[GLFW_KEY_UP] = Keyboard::Key::UP;
	_nativeMap[GLFW_KEY_DOWN] = Keyboard::Key::DOWN;
	_nativeMap[GLFW_KEY_LEFT] = Keyboard::Key::LEFT;
	_nativeMap[GLFW_KEY_RIGHT] = Keyboard::Key::RIGHT;

	_stateMap[Keyboard::Key::UNKNOWN] = Keyboard::KeyState::KEY_RELEASED;
	_stateMap[Keyboard::Key::Z] = Keyboard::KeyState::KEY_RELEASED;
	_stateMap[Keyboard::Key::Q] = Keyboard::KeyState::KEY_RELEASED;
	_stateMap[Keyboard::Key::S] = Keyboard::KeyState::KEY_RELEASED;
	_stateMap[Keyboard::Key::D] = Keyboard::KeyState::KEY_RELEASED;
	_stateMap[Keyboard::Key::UP] = Keyboard::KeyState::KEY_RELEASED;
	_stateMap[Keyboard::Key::DOWN] = Keyboard::KeyState::KEY_RELEASED;
	_stateMap[Keyboard::Key::LEFT] = Keyboard::KeyState::KEY_RELEASED;
	_stateMap[Keyboard::Key::RIGHT] = Keyboard::KeyState::KEY_RELEASED;
}

Keyboard::KeyboardNativeMap&	Keyboard::getNativeMap()
{
	return (_nativeMap);
}

Keyboard::KeyboardStateMap&	Keyboard::getStateMap()
{
	return (_stateMap);
}

Keyboard::KeyState			Keyboard::operator[](Keyboard::Key key)
{
	return (_stateMap[key]);
}