/**
* @Author   Mathieu Chassara
*/

#include <iostream>
#include <GLFW/glfw3.h>

#include <Engine/Window/Keyboard.hpp>

/**
    Some native keys are not directly remapped to their Keyboard clone ones.
    This is because the GLFW library is using an US layout for their keyboard.
    See here : http://www.glfw.org/docs/latest/group__keys.html
*/
Keyboard::Keyboard()
{
	this->_nativeMap[GLFW_KEY_UNKNOWN] = Keyboard::eKey::UNKNOWN;
    this->_nativeMap[GLFW_KEY_SPACE] = Keyboard::eKey::SPACE;
    this->_nativeMap[GLFW_KEY_APOSTROPHE] = Keyboard::eKey::APOSTROPHE;
    this->_nativeMap[GLFW_KEY_COMMA] = Keyboard::eKey::M;
    this->_nativeMap[GLFW_KEY_MINUS] = Keyboard::eKey::MINUS;
    this->_nativeMap[GLFW_KEY_PERIOD] = Keyboard::eKey::PERIOD;
    this->_nativeMap[GLFW_KEY_SLASH] = Keyboard::eKey::SLASH;
    this->_nativeMap[GLFW_KEY_0] = Keyboard::eKey::KEY_0;
    this->_nativeMap[GLFW_KEY_1] = Keyboard::eKey::KEY_1;
    this->_nativeMap[GLFW_KEY_2] = Keyboard::eKey::KEY_2;
    this->_nativeMap[GLFW_KEY_3] = Keyboard::eKey::KEY_3;
    this->_nativeMap[GLFW_KEY_4] = Keyboard::eKey::KEY_4;
    this->_nativeMap[GLFW_KEY_5] = Keyboard::eKey::KEY_5;
    this->_nativeMap[GLFW_KEY_6] = Keyboard::eKey::KEY_6;
    this->_nativeMap[GLFW_KEY_7] = Keyboard::eKey::KEY_7;
    this->_nativeMap[GLFW_KEY_8] = Keyboard::eKey::KEY_8;
    this->_nativeMap[GLFW_KEY_9] = Keyboard::eKey::KEY_9;
    this->_nativeMap[GLFW_KEY_SEMICOLON] = Keyboard::eKey::SEMICOLON;
    this->_nativeMap[GLFW_KEY_EQUAL] = Keyboard::eKey::EQUAL;
    this->_nativeMap[GLFW_KEY_A] = Keyboard::eKey::Q;
    this->_nativeMap[GLFW_KEY_B] = Keyboard::eKey::B;
    this->_nativeMap[GLFW_KEY_C] = Keyboard::eKey::C;
    this->_nativeMap[GLFW_KEY_D] = Keyboard::eKey::D;
    this->_nativeMap[GLFW_KEY_E] = Keyboard::eKey::E;
    this->_nativeMap[GLFW_KEY_F] = Keyboard::eKey::F;
    this->_nativeMap[GLFW_KEY_G] = Keyboard::eKey::G;
    this->_nativeMap[GLFW_KEY_H] = Keyboard::eKey::H;
    this->_nativeMap[GLFW_KEY_I] = Keyboard::eKey::I;
    this->_nativeMap[GLFW_KEY_J] = Keyboard::eKey::J;
    this->_nativeMap[GLFW_KEY_K] = Keyboard::eKey::K;
    this->_nativeMap[GLFW_KEY_L] = Keyboard::eKey::L;
    this->_nativeMap[GLFW_KEY_M] = Keyboard::eKey::M;
    this->_nativeMap[GLFW_KEY_N] = Keyboard::eKey::N;
    this->_nativeMap[GLFW_KEY_O] = Keyboard::eKey::O;
    this->_nativeMap[GLFW_KEY_P] = Keyboard::eKey::P;
    this->_nativeMap[GLFW_KEY_Q] = Keyboard::eKey::A;
    this->_nativeMap[GLFW_KEY_R] = Keyboard::eKey::R;
    this->_nativeMap[GLFW_KEY_S] = Keyboard::eKey::S;
    this->_nativeMap[GLFW_KEY_T] = Keyboard::eKey::T;
    this->_nativeMap[GLFW_KEY_U] = Keyboard::eKey::U;
    this->_nativeMap[GLFW_KEY_V] = Keyboard::eKey::V;
    this->_nativeMap[GLFW_KEY_W] = Keyboard::eKey::Z;
    this->_nativeMap[GLFW_KEY_X] = Keyboard::eKey::X;
    this->_nativeMap[GLFW_KEY_Y] = Keyboard::eKey::Y;
    this->_nativeMap[GLFW_KEY_Z] = Keyboard::eKey::W;
    this->_nativeMap[GLFW_KEY_LEFT_BRACKET] = Keyboard::eKey::LEFT_BRACKET;
    this->_nativeMap[GLFW_KEY_BACKSLASH] = Keyboard::eKey::BACKSLASH;
    this->_nativeMap[GLFW_KEY_RIGHT_BRACKET] = Keyboard::eKey::RIGHT_BRACKET;
    this->_nativeMap[GLFW_KEY_GRAVE_ACCENT] = Keyboard::eKey::GRAVE_ACCENT;
    this->_nativeMap[GLFW_KEY_WORLD_1] = Keyboard::eKey::WORLD_1;
    this->_nativeMap[GLFW_KEY_WORLD_2] = Keyboard::eKey::WORLD_2;
    this->_nativeMap[GLFW_KEY_ESCAPE] = Keyboard::eKey::ESCAPE;
    this->_nativeMap[GLFW_KEY_ENTER] = Keyboard::eKey::ENTER;
    this->_nativeMap[GLFW_KEY_TAB] = Keyboard::eKey::TAB;
    this->_nativeMap[GLFW_KEY_BACKSPACE] = Keyboard::eKey::BACKSPACE;
    this->_nativeMap[GLFW_KEY_INSERT] = Keyboard::eKey::INSERT;
    this->_nativeMap[GLFW_KEY_DELETE] = Keyboard::eKey::DELETE;
    this->_nativeMap[GLFW_KEY_RIGHT] = Keyboard::eKey::RIGHT;
    this->_nativeMap[GLFW_KEY_LEFT] = Keyboard::eKey::LEFT;
    this->_nativeMap[GLFW_KEY_DOWN] = Keyboard::eKey::DOWN;
    this->_nativeMap[GLFW_KEY_UP] = Keyboard::eKey::UP;
    this->_nativeMap[GLFW_KEY_PAGE_UP] = Keyboard::eKey::PAGE_UP;
    this->_nativeMap[GLFW_KEY_PAGE_DOWN] = Keyboard::eKey::PAGE_DOWN;
    this->_nativeMap[GLFW_KEY_HOME] = Keyboard::eKey::HOME;
    this->_nativeMap[GLFW_KEY_END] = Keyboard::eKey::END;
    this->_nativeMap[GLFW_KEY_CAPS_LOCK] = Keyboard::eKey::CAPS_LOCK;
    this->_nativeMap[GLFW_KEY_SCROLL_LOCK] = Keyboard::eKey::SCROLL_LOCK;
    this->_nativeMap[GLFW_KEY_NUM_LOCK] = Keyboard::eKey::NUM_LOCK;
    this->_nativeMap[GLFW_KEY_PRINT_SCREEN] = Keyboard::eKey::PRINT_SCREEN;
    this->_nativeMap[GLFW_KEY_PAUSE] = Keyboard::eKey::PAUSE;
    this->_nativeMap[GLFW_KEY_F1] = Keyboard::eKey::F1;
    this->_nativeMap[GLFW_KEY_F2] = Keyboard::eKey::F2;
    this->_nativeMap[GLFW_KEY_F3] = Keyboard::eKey::F3;
    this->_nativeMap[GLFW_KEY_F4] = Keyboard::eKey::F4;
    this->_nativeMap[GLFW_KEY_F5] = Keyboard::eKey::F5;
    this->_nativeMap[GLFW_KEY_F6] = Keyboard::eKey::F6;
    this->_nativeMap[GLFW_KEY_F7] = Keyboard::eKey::F7;
    this->_nativeMap[GLFW_KEY_F8] = Keyboard::eKey::F8;
    this->_nativeMap[GLFW_KEY_F9] = Keyboard::eKey::F9;
    this->_nativeMap[GLFW_KEY_F10] = Keyboard::eKey::F10;
    this->_nativeMap[GLFW_KEY_F11] = Keyboard::eKey::F11;
    this->_nativeMap[GLFW_KEY_F12] = Keyboard::eKey::F12;
    this->_nativeMap[GLFW_KEY_F13] = Keyboard::eKey::F13;
    this->_nativeMap[GLFW_KEY_F14] = Keyboard::eKey::F14;
    this->_nativeMap[GLFW_KEY_F15] = Keyboard::eKey::F15;
    this->_nativeMap[GLFW_KEY_F16] = Keyboard::eKey::F16;
    this->_nativeMap[GLFW_KEY_F17] = Keyboard::eKey::F17;
    this->_nativeMap[GLFW_KEY_F18] = Keyboard::eKey::F18;
    this->_nativeMap[GLFW_KEY_F19] = Keyboard::eKey::F19;
    this->_nativeMap[GLFW_KEY_F20] = Keyboard::eKey::F20;
    this->_nativeMap[GLFW_KEY_F21] = Keyboard::eKey::F21;
    this->_nativeMap[GLFW_KEY_F22] = Keyboard::eKey::F22;
    this->_nativeMap[GLFW_KEY_F23] = Keyboard::eKey::F23;
    this->_nativeMap[GLFW_KEY_F24] = Keyboard::eKey::F24;
    this->_nativeMap[GLFW_KEY_F25] = Keyboard::eKey::F25;
    this->_nativeMap[GLFW_KEY_KP_0] = Keyboard::eKey::KP_0;
    this->_nativeMap[GLFW_KEY_KP_1] = Keyboard::eKey::KP_1;
    this->_nativeMap[GLFW_KEY_KP_2] = Keyboard::eKey::KP_2;
    this->_nativeMap[GLFW_KEY_KP_3] = Keyboard::eKey::KP_3;
    this->_nativeMap[GLFW_KEY_KP_4] = Keyboard::eKey::KP_4;
    this->_nativeMap[GLFW_KEY_KP_5] = Keyboard::eKey::KP_5;
    this->_nativeMap[GLFW_KEY_KP_6] = Keyboard::eKey::KP_6;
    this->_nativeMap[GLFW_KEY_KP_7] = Keyboard::eKey::KP_7;
    this->_nativeMap[GLFW_KEY_KP_8] = Keyboard::eKey::KP_8;
    this->_nativeMap[GLFW_KEY_KP_9] = Keyboard::eKey::KP_9;
    this->_nativeMap[GLFW_KEY_KP_DECIMAL] = Keyboard::eKey::KP_DECIMAL;
    this->_nativeMap[GLFW_KEY_KP_DIVIDE] = Keyboard::eKey::KP_DIVIDE;
    this->_nativeMap[GLFW_KEY_KP_MULTIPLY] = Keyboard::eKey::KP_MULTIPLY;
    this->_nativeMap[GLFW_KEY_KP_SUBTRACT] = Keyboard::eKey::KP_SUBTRACT;
    this->_nativeMap[GLFW_KEY_KP_ADD] = Keyboard::eKey::KP_ADD;
    this->_nativeMap[GLFW_KEY_KP_ENTER] = Keyboard::eKey::KP_ENTER;
    this->_nativeMap[GLFW_KEY_KP_EQUAL] = Keyboard::eKey::KP_EQUAL;
    this->_nativeMap[GLFW_KEY_LEFT_SHIFT] = Keyboard::eKey::LEFT_SHIFT;
    this->_nativeMap[GLFW_KEY_LEFT_CONTROL] = Keyboard::eKey::LEFT_CONTROL;
    this->_nativeMap[GLFW_KEY_LEFT_ALT] = Keyboard::eKey::LEFT_ALT;
    this->_nativeMap[GLFW_KEY_LEFT_SUPER] = Keyboard::eKey::LEFT_SUPER;
    this->_nativeMap[GLFW_KEY_RIGHT_SHIFT] = Keyboard::eKey::RIGHT_SHIFT;
    this->_nativeMap[GLFW_KEY_RIGHT_CONTROL] = Keyboard::eKey::RIGHT_CONTROL;
    this->_nativeMap[GLFW_KEY_RIGHT_ALT] = Keyboard::eKey::RIGHT_ALT;
    this->_nativeMap[GLFW_KEY_RIGHT_SUPER] = Keyboard::eKey::RIGHT_SUPER;
    this->_nativeMap[GLFW_KEY_MENU] = Keyboard::eKey::MENU;
    this->_nativeMap[GLFW_KEY_LAST] = Keyboard::eKey::LAST;

    this->_stringMap["UNKNOWN"] = Keyboard::eKey::UNKNOWN;
    this->_stringMap["SPACE"] = Keyboard::eKey::SPACE;
    this->_stringMap["APOSTROPHE"] = Keyboard::eKey::APOSTROPHE;
    this->_stringMap["COMMA"] = Keyboard::eKey::M;
    this->_stringMap["MINUS"] = Keyboard::eKey::MINUS;
    this->_stringMap["PERIOD"] = Keyboard::eKey::PERIOD;
    this->_stringMap["SLASH"] = Keyboard::eKey::SLASH;
    this->_stringMap["0"] = Keyboard::eKey::KEY_0;
    this->_stringMap["1"] = Keyboard::eKey::KEY_1;
    this->_stringMap["2"] = Keyboard::eKey::KEY_2;
    this->_stringMap["3"] = Keyboard::eKey::KEY_3;
    this->_stringMap["4"] = Keyboard::eKey::KEY_4;
    this->_stringMap["5"] = Keyboard::eKey::KEY_5;
    this->_stringMap["6"] = Keyboard::eKey::KEY_6;
    this->_stringMap["7"] = Keyboard::eKey::KEY_7;
    this->_stringMap["8"] = Keyboard::eKey::KEY_8;
    this->_stringMap["9"] = Keyboard::eKey::KEY_9;
    this->_stringMap["SEMICOLON"] = Keyboard::eKey::SEMICOLON;
    this->_stringMap["EQUAL"] = Keyboard::eKey::EQUAL;
    this->_stringMap["A"] = Keyboard::eKey::A;
    this->_stringMap["B"] = Keyboard::eKey::B;
    this->_stringMap["C"] = Keyboard::eKey::C;
    this->_stringMap["D"] = Keyboard::eKey::D;
    this->_stringMap["E"] = Keyboard::eKey::E;
    this->_stringMap["F"] = Keyboard::eKey::F;
    this->_stringMap["G"] = Keyboard::eKey::G;
    this->_stringMap["H"] = Keyboard::eKey::H;
    this->_stringMap["I"] = Keyboard::eKey::I;
    this->_stringMap["J"] = Keyboard::eKey::J;
    this->_stringMap["K"] = Keyboard::eKey::K;
    this->_stringMap["L"] = Keyboard::eKey::L;
    this->_stringMap["M"] = Keyboard::eKey::M;
    this->_stringMap["N"] = Keyboard::eKey::N;
    this->_stringMap["O"] = Keyboard::eKey::O;
    this->_stringMap["P"] = Keyboard::eKey::P;
    this->_stringMap["Q"] = Keyboard::eKey::Q;
    this->_stringMap["R"] = Keyboard::eKey::R;
    this->_stringMap["S"] = Keyboard::eKey::S;
    this->_stringMap["T"] = Keyboard::eKey::T;
    this->_stringMap["U"] = Keyboard::eKey::U;
    this->_stringMap["V"] = Keyboard::eKey::V;
    this->_stringMap["W"] = Keyboard::eKey::W;
    this->_stringMap["X"] = Keyboard::eKey::X;
    this->_stringMap["Y"] = Keyboard::eKey::Y;
    this->_stringMap["Z"] = Keyboard::eKey::Z;
    this->_stringMap["LEFT_BRACKET"] = Keyboard::eKey::LEFT_BRACKET;
    this->_stringMap["BACKSLASH"] = Keyboard::eKey::BACKSLASH;
    this->_stringMap["RIGHT_BRACKET"] = Keyboard::eKey::RIGHT_BRACKET;
    this->_stringMap["GRAVE_ACCENT"] = Keyboard::eKey::GRAVE_ACCENT;
    this->_stringMap["WORLD_1"] = Keyboard::eKey::WORLD_1;
    this->_stringMap["WORLD_2"] = Keyboard::eKey::WORLD_2;
    this->_stringMap["ESCAPE"] = Keyboard::eKey::ESCAPE;
    this->_stringMap["ENTER"] = Keyboard::eKey::ENTER;
    this->_stringMap["TAB"] = Keyboard::eKey::TAB;
    this->_stringMap["BACKSPACE"] = Keyboard::eKey::BACKSPACE;
    this->_stringMap["INSERT"] = Keyboard::eKey::INSERT;
    this->_stringMap["DELETE"] = Keyboard::eKey::DELETE;
    this->_stringMap["RIGHT"] = Keyboard::eKey::RIGHT;
    this->_stringMap["LEFT"] = Keyboard::eKey::LEFT;
    this->_stringMap["DOWN"] = Keyboard::eKey::DOWN;
    this->_stringMap["UP"] = Keyboard::eKey::UP;
    this->_stringMap["PAGE_UP"] = Keyboard::eKey::PAGE_UP;
    this->_stringMap["PAGE_DOWN"] = Keyboard::eKey::PAGE_DOWN;
    this->_stringMap["HOME"] = Keyboard::eKey::HOME;
    this->_stringMap["END"] = Keyboard::eKey::END;
    this->_stringMap["CAPS_LOCK"] = Keyboard::eKey::CAPS_LOCK;
    this->_stringMap["SCROLL_LOCK"] = Keyboard::eKey::SCROLL_LOCK;
    this->_stringMap["NUM_LOCK"] = Keyboard::eKey::NUM_LOCK;
    this->_stringMap["PRINT_SCREEN"] = Keyboard::eKey::PRINT_SCREEN;
    this->_stringMap["PAUSE"] = Keyboard::eKey::PAUSE;
    this->_stringMap["F1"] = Keyboard::eKey::F1;
    this->_stringMap["F2"] = Keyboard::eKey::F2;
    this->_stringMap["F3"] = Keyboard::eKey::F3;
    this->_stringMap["F4"] = Keyboard::eKey::F4;
    this->_stringMap["F5"] = Keyboard::eKey::F5;
    this->_stringMap["F6"] = Keyboard::eKey::F6;
    this->_stringMap["F7"] = Keyboard::eKey::F7;
    this->_stringMap["F8"] = Keyboard::eKey::F8;
    this->_stringMap["F9"] = Keyboard::eKey::F9;
    this->_stringMap["F10"] = Keyboard::eKey::F10;
    this->_stringMap["F11"] = Keyboard::eKey::F11;
    this->_stringMap["F12"] = Keyboard::eKey::F12;
    this->_stringMap["F13"] = Keyboard::eKey::F13;
    this->_stringMap["F14"] = Keyboard::eKey::F14;
    this->_stringMap["F15"] = Keyboard::eKey::F15;
    this->_stringMap["F16"] = Keyboard::eKey::F16;
    this->_stringMap["F17"] = Keyboard::eKey::F17;
    this->_stringMap["F18"] = Keyboard::eKey::F18;
    this->_stringMap["F19"] = Keyboard::eKey::F19;
    this->_stringMap["F20"] = Keyboard::eKey::F20;
    this->_stringMap["F21"] = Keyboard::eKey::F21;
    this->_stringMap["F22"] = Keyboard::eKey::F22;
    this->_stringMap["F23"] = Keyboard::eKey::F23;
    this->_stringMap["F24"] = Keyboard::eKey::F24;
    this->_stringMap["F25"] = Keyboard::eKey::F25;
    this->_stringMap["KP_0"] = Keyboard::eKey::KP_0;
    this->_stringMap["KP_1"] = Keyboard::eKey::KP_1;
    this->_stringMap["KP_2"] = Keyboard::eKey::KP_2;
    this->_stringMap["KP_3"] = Keyboard::eKey::KP_3;
    this->_stringMap["KP_4"] = Keyboard::eKey::KP_4;
    this->_stringMap["KP_5"] = Keyboard::eKey::KP_5;
    this->_stringMap["KP_6"] = Keyboard::eKey::KP_6;
    this->_stringMap["KP_7"] = Keyboard::eKey::KP_7;
    this->_stringMap["KP_8"] = Keyboard::eKey::KP_8;
    this->_stringMap["KP_9"] = Keyboard::eKey::KP_9;
    this->_stringMap["KP_DECIMAL"] = Keyboard::eKey::KP_DECIMAL;
    this->_stringMap["KP_DIVIDE"] = Keyboard::eKey::KP_DIVIDE;
    this->_stringMap["KP_MULTIPLY"] = Keyboard::eKey::KP_MULTIPLY;
    this->_stringMap["KP_SUBTRACT"] = Keyboard::eKey::KP_SUBTRACT;
    this->_stringMap["KP_ADD"] = Keyboard::eKey::KP_ADD;
    this->_stringMap["KP_ENTER"] = Keyboard::eKey::KP_ENTER;
    this->_stringMap["KP_EQUAL"] = Keyboard::eKey::KP_EQUAL;
    this->_stringMap["LEFT_SHIFT"] = Keyboard::eKey::LEFT_SHIFT;
    this->_stringMap["LEFT_CONTROL"] = Keyboard::eKey::LEFT_CONTROL;
    this->_stringMap["LEFT_ALT"] = Keyboard::eKey::LEFT_ALT;
    this->_stringMap["LEFT_SUPER"] = Keyboard::eKey::LEFT_SUPER;
    this->_stringMap["RIGHT_SHIFT"] = Keyboard::eKey::RIGHT_SHIFT;
    this->_stringMap["RIGHT_CONTROL"] = Keyboard::eKey::RIGHT_CONTROL;
    this->_stringMap["RIGHT_ALT"] = Keyboard::eKey::RIGHT_ALT;
    this->_stringMap["RIGHT_SUPER"] = Keyboard::eKey::RIGHT_SUPER;
    this->_stringMap["MENU"] = Keyboard::eKey::MENU;
    this->_stringMap["LAST"] = Keyboard::eKey::LAST;

    this->resetKeyboardState();
}

Keyboard::KeyboardNativeMap&    Keyboard::getNativeMap()
{
	return (_nativeMap);
}

Keyboard::KeyboardStateMap& Keyboard::getStateMap()
{
	return (_stateMap);
}

Keyboard::KeyboardStringMap& Keyboard::getStringMap()
{
    return (_stringMap);
}

std::string Keyboard::keyToString(Keyboard::eKey key)
{
    for (auto it = _stringMap.begin(); it != _stringMap.end(); it++)
    {
        if (it->second == key)
            return (it->first);
    }
    return ("");
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

void    Keyboard::updateKeyboardState()
{
    Keyboard::KeyboardNativeMap::const_iterator it;

    for (it = _nativeMap.begin(); it != _nativeMap.end(); ++it)
    {
        switch (_stateMap[it->second])
        {
            case Keyboard::eKeyState::KEY_RELEASED:
                _stateMap[it->second] = Keyboard::eKeyState::KEY_IDLE;
                break;
            case Keyboard::eKeyState::KEY_PRESSED:
                _stateMap[it->second] = Keyboard::eKeyState::KEY_MAINTAINED;
                break;
            default:
                break;
        }
    }
}

bool    Keyboard::isPressed(Keyboard::eKey key)
{
    return (_stateMap[key] == Keyboard::eKeyState::KEY_PRESSED ||
            _stateMap[key] == Keyboard::eKeyState::KEY_MAINTAINED);
}
