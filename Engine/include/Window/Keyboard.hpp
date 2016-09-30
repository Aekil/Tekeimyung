#pragma once

#include <unordered_map>

class Keyboard
{
public:
	enum class			Key
	{
		UNKNOWN = -1,	// Unknown	key
		Z = 0,			// Z		key
		Q,				// Q		key
		S,				// S		key
		D,				// D		key
		UP,				// Up		key
		DOWN,			// Down		key
		LEFT,			// Left		key
		RIGHT			// Right	key
	};

	enum class			KeyState
	{
		KEY_PRESSED = 0,	// The key has just been pressed !
		KEY_MAINTAINED,		// The key has been pressed in the previous frame and still is in the current frame.
		KEY_RELEASED		// The key has just been released !
	};
private:
	typedef std::unordered_map<int, Keyboard::Key>					KeyboardNativeMap;
	typedef std::unordered_map<Keyboard::Key, Keyboard::KeyState>	KeyboardStateMap;
public:
	explicit			Keyboard();
	virtual				~Keyboard() {}

	KeyboardNativeMap&	getNativeMap();
	KeyboardStateMap&	getStateMap();
	Keyboard::KeyState	operator[](Keyboard::Key key);
private:
	KeyboardNativeMap	_nativeMap;
	KeyboardStateMap	_stateMap;
};