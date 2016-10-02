#include <iostream>
#include <GLFW/glfw3.h>

#include <Window/Keyboard.hpp>

/**
    Some native keys are not directly remapped to their Keyboard clone ones.
    This is because the GLFW library is using an US layout for their keyboard.
    See here : http://www.glfw.org/docs/latest/group__keys.html
*/
Keyboard::Keyboard()
{
	_nativeMap[GLFW_KEY_UNKNOWN] = Keyboard::eKey::UNKNOWN;
    _nativeMap[GLFW_KEY_SPACE] = Keyboard::eKey::SPACE;
    //_nativeMap[GLFW_KEY_APOSTROPHE] = ;
    _nativeMap[GLFW_KEY_COMMA] = Keyboard::eKey::M;
    //_nativeMap[GLFW_KEY_MINUS] = ;
    //_nativeMap[GLFW_KEY_PERIOD] = ;
    //_nativeMap[GLFW_KEY_SLASH] = ;
    _nativeMap[GLFW_KEY_0] = Keyboard::eKey::KEY_0;
    _nativeMap[GLFW_KEY_1] = Keyboard::eKey::KEY_1;
    _nativeMap[GLFW_KEY_2] = Keyboard::eKey::KEY_2;
    _nativeMap[GLFW_KEY_3] = Keyboard::eKey::KEY_3;
    _nativeMap[GLFW_KEY_4] = Keyboard::eKey::KEY_4;
    _nativeMap[GLFW_KEY_5] = Keyboard::eKey::KEY_5;
    _nativeMap[GLFW_KEY_6] = Keyboard::eKey::KEY_6;
    _nativeMap[GLFW_KEY_7] = Keyboard::eKey::KEY_7;
    _nativeMap[GLFW_KEY_8] = Keyboard::eKey::KEY_8;
    _nativeMap[GLFW_KEY_9] = Keyboard::eKey::KEY_9;
    //_nativeMap[GLFW_KEY_SEMICOLON] = ;
    //_nativeMap[GLFW_KEY_EQUAL] = ;
    _nativeMap[GLFW_KEY_A] = Keyboard::eKey::Q;
    _nativeMap[GLFW_KEY_B] = Keyboard::eKey::B;
    _nativeMap[GLFW_KEY_C] = Keyboard::eKey::C;
    _nativeMap[GLFW_KEY_D] = Keyboard::eKey::D;
    _nativeMap[GLFW_KEY_E] = Keyboard::eKey::E;
    _nativeMap[GLFW_KEY_F] = Keyboard::eKey::F;
    _nativeMap[GLFW_KEY_G] = Keyboard::eKey::G;
    _nativeMap[GLFW_KEY_H] = Keyboard::eKey::H;
    _nativeMap[GLFW_KEY_I] = Keyboard::eKey::I;
    _nativeMap[GLFW_KEY_J] = Keyboard::eKey::J;
    _nativeMap[GLFW_KEY_K] = Keyboard::eKey::K;
    _nativeMap[GLFW_KEY_L] = Keyboard::eKey::L;
    _nativeMap[GLFW_KEY_M] = Keyboard::eKey::COMMA;
    _nativeMap[GLFW_KEY_N] = Keyboard::eKey::N;
    _nativeMap[GLFW_KEY_O] = Keyboard::eKey::O;
    _nativeMap[GLFW_KEY_P] = Keyboard::eKey::P;
    _nativeMap[GLFW_KEY_Q] = Keyboard::eKey::A;
    _nativeMap[GLFW_KEY_R] = Keyboard::eKey::R;
    _nativeMap[GLFW_KEY_S] = Keyboard::eKey::S;
    _nativeMap[GLFW_KEY_T] = Keyboard::eKey::T;
    _nativeMap[GLFW_KEY_U] = Keyboard::eKey::U;
    _nativeMap[GLFW_KEY_V] = Keyboard::eKey::V;
    _nativeMap[GLFW_KEY_W] = Keyboard::eKey::Z;
    _nativeMap[GLFW_KEY_X] = Keyboard::eKey::X;
    _nativeMap[GLFW_KEY_Y] = Keyboard::eKey::Y;
    _nativeMap[GLFW_KEY_Z] = Keyboard::eKey::Z;
    //_nativeMap[GLFW_KEY_LEFT_BRACKET] = ;
    //_nativeMap[GLFW_KEY_BACKSLASH] = ;
    //_nativeMap[GLFW_KEY_RIGHT_BRACKET] = ;
    //_nativeMap[GLFW_KEY_GRAVE_ACCENT] = ;
    //_nativeMap[GLFW_KEY_WORLD_1] = ;
    //_nativeMap[GLFW_KEY_WORLD_2] = ;
    _nativeMap[GLFW_KEY_ESCAPE] = Keyboard::eKey::ESCAPE;
    _nativeMap[GLFW_KEY_ENTER] = Keyboard::eKey::ENTER;
    _nativeMap[GLFW_KEY_TAB] = Keyboard::eKey::TAB;
    _nativeMap[GLFW_KEY_BACKSPACE] = Keyboard::eKey::BACKSPACE;
    _nativeMap[GLFW_KEY_INSERT] = Keyboard::eKey::INSERT;
    _nativeMap[GLFW_KEY_DELETE] = Keyboard::eKey::DELETE;
    _nativeMap[GLFW_KEY_RIGHT] = Keyboard::eKey::RIGHT;
    _nativeMap[GLFW_KEY_LEFT] = Keyboard::eKey::LEFT;
    _nativeMap[GLFW_KEY_DOWN] = Keyboard::eKey::DOWN;
    _nativeMap[GLFW_KEY_UP] = Keyboard::eKey::UP;
    _nativeMap[GLFW_KEY_PAGE_UP] = Keyboard::eKey::PAGE_UP;
    _nativeMap[GLFW_KEY_PAGE_DOWN] = Keyboard::eKey::PAGE_DOWN;
    //_nativeMap[GLFW_KEY_HOME] = ;
    //_nativeMap[GLFW_KEY_END] = ;
    _nativeMap[GLFW_KEY_CAPS_LOCK] = Keyboard::eKey::CAPS_LOCK;
    //_nativeMap[GLFW_KEY_SCROLL_LOCK] = ;
    _nativeMap[GLFW_KEY_NUM_LOCK] = Keyboard::eKey::NUM_LOCK;
    _nativeMap[GLFW_KEY_PRINT_SCREEN] = Keyboard::eKey::PRINT_SCREEN;
    _nativeMap[GLFW_KEY_PAUSE] = Keyboard::eKey::PAUSE;
    _nativeMap[GLFW_KEY_F1] = Keyboard::eKey::F1;
    _nativeMap[GLFW_KEY_F2] = Keyboard::eKey::F2;
    _nativeMap[GLFW_KEY_F3] = Keyboard::eKey::F3;
    _nativeMap[GLFW_KEY_F4] = Keyboard::eKey::F4;
    _nativeMap[GLFW_KEY_F5] = Keyboard::eKey::F5;
    _nativeMap[GLFW_KEY_F6] = Keyboard::eKey::F6;
    _nativeMap[GLFW_KEY_F7] = Keyboard::eKey::F7;
    _nativeMap[GLFW_KEY_F8] = Keyboard::eKey::F8;
    _nativeMap[GLFW_KEY_F9] = Keyboard::eKey::F9;
    _nativeMap[GLFW_KEY_F10] = Keyboard::eKey::F10;
    _nativeMap[GLFW_KEY_F11] = Keyboard::eKey::F11;
    _nativeMap[GLFW_KEY_F12] = Keyboard::eKey::F12;
    //_nativeMap[GLFW_KEY_F13] = ;
    //_nativeMap[GLFW_KEY_F14] = ;
    //_nativeMap[GLFW_KEY_F15] = ;
    //_nativeMap[GLFW_KEY_F16] = ;
    //_nativeMap[GLFW_KEY_F17] = ;
    //_nativeMap[GLFW_KEY_F18] = ;
    //_nativeMap[GLFW_KEY_F19] = ;
    //_nativeMap[GLFW_KEY_F20] = ;
    //_nativeMap[GLFW_KEY_F21] = ;
    //_nativeMap[GLFW_KEY_F22] = ;
    //_nativeMap[GLFW_KEY_F23] = ;
    //_nativeMap[GLFW_KEY_F24] = ;
    //_nativeMap[GLFW_KEY_F25] = ;
    _nativeMap[GLFW_KEY_KP_0] = Keyboard::eKey::KP_0;
    _nativeMap[GLFW_KEY_KP_1] = Keyboard::eKey::KP_1;
    _nativeMap[GLFW_KEY_KP_2] = Keyboard::eKey::KP_2;
    _nativeMap[GLFW_KEY_KP_3] = Keyboard::eKey::KP_3;
    _nativeMap[GLFW_KEY_KP_4] = Keyboard::eKey::KP_4;
    _nativeMap[GLFW_KEY_KP_5] = Keyboard::eKey::KP_5;
    _nativeMap[GLFW_KEY_KP_6] = Keyboard::eKey::KP_6;
    _nativeMap[GLFW_KEY_KP_7] = Keyboard::eKey::KP_7;
    _nativeMap[GLFW_KEY_KP_8] = Keyboard::eKey::KP_8;
    _nativeMap[GLFW_KEY_KP_9] = Keyboard::eKey::KP_9;
    _nativeMap[GLFW_KEY_KP_DECIMAL] = Keyboard::eKey::KP_DECIMAL;
    _nativeMap[GLFW_KEY_KP_DIVIDE] = Keyboard::eKey::KP_DIVIDE;
    _nativeMap[GLFW_KEY_KP_MULTIPLY] = Keyboard::eKey::KP_MULTIPLY;
    _nativeMap[GLFW_KEY_KP_SUBTRACT] = Keyboard::eKey::KP_SUBTRACT;
    _nativeMap[GLFW_KEY_KP_ADD] = Keyboard::eKey::KP_ADD;
    _nativeMap[GLFW_KEY_KP_ENTER] = Keyboard::eKey::KP_ENTER;
    _nativeMap[GLFW_KEY_KP_EQUAL] = Keyboard::eKey::KP_EQUAL;
    _nativeMap[GLFW_KEY_LEFT_SHIFT] = Keyboard::eKey::LEFT_SHIFT;
    _nativeMap[GLFW_KEY_LEFT_CONTROL] = Keyboard::eKey::LEFT_CONTROL;
    _nativeMap[GLFW_KEY_LEFT_ALT] = Keyboard::eKey::LEFT_ALT;
    _nativeMap[GLFW_KEY_LEFT_SUPER] = Keyboard::eKey::LEFT_SUPER;
    _nativeMap[GLFW_KEY_RIGHT_SHIFT] = Keyboard::eKey::RIGHT_SHIFT;
    _nativeMap[GLFW_KEY_RIGHT_CONTROL] = Keyboard::eKey::RIGHT_CONTROL;
    _nativeMap[GLFW_KEY_RIGHT_ALT] = Keyboard::eKey::RIGHT_ALT;
    _nativeMap[GLFW_KEY_RIGHT_SUPER] = Keyboard::eKey::RIGHT_SUPER;
    //_nativeMap[GLFW_KEY_MENU] = ;
    //_nativeMap[GLFW_KEY_LAST] = _nativeMap[GLFW_KEY_MENU];

    resetKeyboardState();
	/*_stateMap[Keyboard::eKey::UNKNOWN] = Keyboard::eKeyState::KEY_RELEASED;
	_stateMap[Keyboard::eKey::Z] = Keyboard::eKeyState::KEY_RELEASED;
	_stateMap[Keyboard::eKey::Q] = Keyboard::eKeyState::KEY_RELEASED;
	_stateMap[Keyboard::eKey::S] = Keyboard::eKeyState::KEY_RELEASED;
	_stateMap[Keyboard::eKey::D] = Keyboard::eKeyState::KEY_RELEASED;
	_stateMap[Keyboard::eKey::UP] = Keyboard::eKeyState::KEY_RELEASED;
	_stateMap[Keyboard::eKey::DOWN] = Keyboard::eKeyState::KEY_RELEASED;
	_stateMap[Keyboard::eKey::LEFT] = Keyboard::eKeyState::KEY_RELEASED;
	_stateMap[Keyboard::eKey::RIGHT] = Keyboard::eKeyState::KEY_RELEASED;*/
}

Keyboard::KeyboardNativeMap&    Keyboard::getNativeMap()
{
	return (_nativeMap);
}

Keyboard::KeyboardStateMap& Keyboard::getStateMap()
{
	return (_stateMap);
}

Keyboard::eKeyState Keyboard::operator[](Keyboard::eKey key)
{
	return (_stateMap[key]);
}

void    Keyboard::resetKeyboardState()
{
    Keyboard::KeyboardNativeMap::const_iterator it;

    for (it = _nativeMap.begin(); it != _nativeMap.end(); ++it)
    {
        _stateMap[it->second] = Keyboard::eKeyState::KEY_IDLE;
    }
}

void    Keyboard::resetReleasedKeys()
{
    Keyboard::KeyboardNativeMap::const_iterator it;

    for (it = _nativeMap.begin(); it != _nativeMap.end(); ++it)
    {
        if (_stateMap[it->second] == Keyboard::eKeyState::KEY_RELEASED)
            _stateMap[it->second] = Keyboard::eKeyState::KEY_IDLE;
    }
}

bool    Keyboard::isPressed(Keyboard::eKey key)
{
    return (_stateMap[key] == Keyboard::eKeyState::KEY_PRESSED ||
            _stateMap[key] == Keyboard::eKeyState::KEY_MAINTAINED);
}