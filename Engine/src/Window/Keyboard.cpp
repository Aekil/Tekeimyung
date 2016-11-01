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
    _nativeMap[GLFW_KEY_APOSTROPHE] = Keyboard::eKey::APOSTROPHE;
    _nativeMap[GLFW_KEY_COMMA] = Keyboard::eKey::M;
    _nativeMap[GLFW_KEY_MINUS] = Keyboard::eKey::MINUS;
    _nativeMap[GLFW_KEY_PERIOD] = Keyboard::eKey::PERIOD;
    _nativeMap[GLFW_KEY_SLASH] = Keyboard::eKey::SLASH;
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
    _nativeMap[GLFW_KEY_SEMICOLON] = Keyboard::eKey::SEMICOLON;
    _nativeMap[GLFW_KEY_EQUAL] = Keyboard::eKey::EQUAL;
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
    _nativeMap[GLFW_KEY_LEFT_BRACKET] = Keyboard::eKey::LEFT_BRACKET;
    _nativeMap[GLFW_KEY_BACKSLASH] = Keyboard::eKey::BACKSLASH;
    _nativeMap[GLFW_KEY_RIGHT_BRACKET] = Keyboard::eKey::RIGHT_BRACKET;
    _nativeMap[GLFW_KEY_GRAVE_ACCENT] = Keyboard::eKey::GRAVE_ACCENT;
    _nativeMap[GLFW_KEY_WORLD_1] = Keyboard::eKey::WORLD_1;
    _nativeMap[GLFW_KEY_WORLD_2] = Keyboard::eKey::WORLD_2;
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
    _nativeMap[GLFW_KEY_HOME] = Keyboard::eKey::HOME;
    _nativeMap[GLFW_KEY_END] = Keyboard::eKey::END;
    _nativeMap[GLFW_KEY_CAPS_LOCK] = Keyboard::eKey::CAPS_LOCK;
    _nativeMap[GLFW_KEY_SCROLL_LOCK] = Keyboard::eKey::SCROLL_LOCK;
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
    _nativeMap[GLFW_KEY_F13] = Keyboard::eKey::F13;
    _nativeMap[GLFW_KEY_F14] = Keyboard::eKey::F14;
    _nativeMap[GLFW_KEY_F15] = Keyboard::eKey::F15;
    _nativeMap[GLFW_KEY_F16] = Keyboard::eKey::F16;
    _nativeMap[GLFW_KEY_F17] = Keyboard::eKey::F17;
    _nativeMap[GLFW_KEY_F18] = Keyboard::eKey::F18;
    _nativeMap[GLFW_KEY_F19] = Keyboard::eKey::F19;
    _nativeMap[GLFW_KEY_F20] = Keyboard::eKey::F20;
    _nativeMap[GLFW_KEY_F21] = Keyboard::eKey::F21;
    _nativeMap[GLFW_KEY_F22] = Keyboard::eKey::F22;
    _nativeMap[GLFW_KEY_F23] = Keyboard::eKey::F23;
    _nativeMap[GLFW_KEY_F24] = Keyboard::eKey::F24;
    _nativeMap[GLFW_KEY_F25] = Keyboard::eKey::F25;
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
    _nativeMap[GLFW_KEY_MENU] = Keyboard::eKey::MENU;
    _nativeMap[GLFW_KEY_LAST] = Keyboard::eKey::LAST;

    _stringMap["UNKNOWN"] = Keyboard::eKey::UNKNOWN;
    _stringMap["SPACE"] = Keyboard::eKey::SPACE;
    _stringMap["APOSTROPHE"] = Keyboard::eKey::APOSTROPHE;
    _stringMap["COMMA"] = Keyboard::eKey::M;
    _stringMap["MINUS"] = Keyboard::eKey::MINUS;
    _stringMap["PERIOD"] = Keyboard::eKey::PERIOD;
    _stringMap["SLASH"] = Keyboard::eKey::SLASH;
    _stringMap["0"] = Keyboard::eKey::KEY_0;
    _stringMap["1"] = Keyboard::eKey::KEY_1;
    _stringMap["2"] = Keyboard::eKey::KEY_2;
    _stringMap["3"] = Keyboard::eKey::KEY_3;
    _stringMap["4"] = Keyboard::eKey::KEY_4;
    _stringMap["5"] = Keyboard::eKey::KEY_5;
    _stringMap["6"] = Keyboard::eKey::KEY_6;
    _stringMap["7"] = Keyboard::eKey::KEY_7;
    _stringMap["8"] = Keyboard::eKey::KEY_8;
    _stringMap["9"] = Keyboard::eKey::KEY_9;
    _stringMap["SEMICOLON"] = Keyboard::eKey::SEMICOLON;
    _stringMap["EQUAL"] = Keyboard::eKey::EQUAL;
    _stringMap["A"] = Keyboard::eKey::A;
    _stringMap["B"] = Keyboard::eKey::B;
    _stringMap["C"] = Keyboard::eKey::C;
    _stringMap["D"] = Keyboard::eKey::D;
    _stringMap["E"] = Keyboard::eKey::E;
    _stringMap["F"] = Keyboard::eKey::F;
    _stringMap["G"] = Keyboard::eKey::G;
    _stringMap["H"] = Keyboard::eKey::H;
    _stringMap["I"] = Keyboard::eKey::I;
    _stringMap["J"] = Keyboard::eKey::J;
    _stringMap["K"] = Keyboard::eKey::K;
    _stringMap["L"] = Keyboard::eKey::L;
    _stringMap["M"] = Keyboard::eKey::M;
    _stringMap["N"] = Keyboard::eKey::N;
    _stringMap["O"] = Keyboard::eKey::O;
    _stringMap["P"] = Keyboard::eKey::P;
    _stringMap["Q"] = Keyboard::eKey::Q;
    _stringMap["R"] = Keyboard::eKey::R;
    _stringMap["S"] = Keyboard::eKey::S;
    _stringMap["T"] = Keyboard::eKey::T;
    _stringMap["U"] = Keyboard::eKey::U;
    _stringMap["V"] = Keyboard::eKey::V;
    _stringMap["W"] = Keyboard::eKey::W;
    _stringMap["X"] = Keyboard::eKey::X;
    _stringMap["Y"] = Keyboard::eKey::Y;
    _stringMap["Z"] = Keyboard::eKey::Z;
    _stringMap["LEFT_BRACKET"] = Keyboard::eKey::LEFT_BRACKET;
    _stringMap["BACKSLASH"] = Keyboard::eKey::BACKSLASH;
    _stringMap["RIGHT_BRACKET"] = Keyboard::eKey::RIGHT_BRACKET;
    _stringMap["GRAVE_ACCENT"] = Keyboard::eKey::GRAVE_ACCENT;
    _stringMap["WORLD_1"] = Keyboard::eKey::WORLD_1;
    _stringMap["WORLD_2"] = Keyboard::eKey::WORLD_2;
    _stringMap["ESCAPE"] = Keyboard::eKey::ESCAPE;
    _stringMap["ENTER"] = Keyboard::eKey::ENTER;
    _stringMap["TAB"] = Keyboard::eKey::TAB;
    _stringMap["BACKSPACE"] = Keyboard::eKey::BACKSPACE;
    _stringMap["INSERT"] = Keyboard::eKey::INSERT;
    _stringMap["DELETE"] = Keyboard::eKey::DELETE;
    _stringMap["RIGHT"] = Keyboard::eKey::RIGHT;
    _stringMap["LEFT"] = Keyboard::eKey::LEFT;
    _stringMap["DOWN"] = Keyboard::eKey::DOWN;
    _stringMap["UP"] = Keyboard::eKey::UP;
    _stringMap["PAGE_UP"] = Keyboard::eKey::PAGE_UP;
    _stringMap["PAGE_DOWN"] = Keyboard::eKey::PAGE_DOWN;
    _stringMap["HOME"] = Keyboard::eKey::HOME;
    _stringMap["END"] = Keyboard::eKey::END;
    _stringMap["CAPS_LOCK"] = Keyboard::eKey::CAPS_LOCK;
    _stringMap["SCROLL_LOCK"] = Keyboard::eKey::SCROLL_LOCK;
    _stringMap["NUM_LOCK"] = Keyboard::eKey::NUM_LOCK;
    _stringMap["PRINT_SCREEN"] = Keyboard::eKey::PRINT_SCREEN;
    _stringMap["PAUSE"] = Keyboard::eKey::PAUSE;
    _stringMap["F1"] = Keyboard::eKey::F1;
    _stringMap["F2"] = Keyboard::eKey::F2;
    _stringMap["F3"] = Keyboard::eKey::F3;
    _stringMap["F4"] = Keyboard::eKey::F4;
    _stringMap["F5"] = Keyboard::eKey::F5;
    _stringMap["F6"] = Keyboard::eKey::F6;
    _stringMap["F7"] = Keyboard::eKey::F7;
    _stringMap["F8"] = Keyboard::eKey::F8;
    _stringMap["F9"] = Keyboard::eKey::F9;
    _stringMap["F10"] = Keyboard::eKey::F10;
    _stringMap["F11"] = Keyboard::eKey::F11;
    _stringMap["F12"] = Keyboard::eKey::F12;
    _stringMap["F13"] = Keyboard::eKey::F13;
    _stringMap["F14"] = Keyboard::eKey::F14;
    _stringMap["F15"] = Keyboard::eKey::F15;
    _stringMap["F16"] = Keyboard::eKey::F16;
    _stringMap["F17"] = Keyboard::eKey::F17;
    _stringMap["F18"] = Keyboard::eKey::F18;
    _stringMap["F19"] = Keyboard::eKey::F19;
    _stringMap["F20"] = Keyboard::eKey::F20;
    _stringMap["F21"] = Keyboard::eKey::F21;
    _stringMap["F22"] = Keyboard::eKey::F22;
    _stringMap["F23"] = Keyboard::eKey::F23;
    _stringMap["F24"] = Keyboard::eKey::F24;
    _stringMap["F25"] = Keyboard::eKey::F25;
    _stringMap["KP_0"] = Keyboard::eKey::KP_0;
    _stringMap["KP_1"] = Keyboard::eKey::KP_1;
    _stringMap["KP_2"] = Keyboard::eKey::KP_2;
    _stringMap["KP_3"] = Keyboard::eKey::KP_3;
    _stringMap["KP_4"] = Keyboard::eKey::KP_4;
    _stringMap["KP_5"] = Keyboard::eKey::KP_5;
    _stringMap["KP_6"] = Keyboard::eKey::KP_6;
    _stringMap["KP_7"] = Keyboard::eKey::KP_7;
    _stringMap["KP_8"] = Keyboard::eKey::KP_8;
    _stringMap["KP_9"] = Keyboard::eKey::KP_9;
    _stringMap["KP_DECIMAL"] = Keyboard::eKey::KP_DECIMAL;
    _stringMap["KP_DIVIDE"] = Keyboard::eKey::KP_DIVIDE;
    _stringMap["KP_MULTIPLY"] = Keyboard::eKey::KP_MULTIPLY;
    _stringMap["KP_SUBTRACT"] = Keyboard::eKey::KP_SUBTRACT;
    _stringMap["KP_ADD"] = Keyboard::eKey::KP_ADD;
    _stringMap["KP_ENTER"] = Keyboard::eKey::KP_ENTER;
    _stringMap["KP_EQUAL"] = Keyboard::eKey::KP_EQUAL;
    _stringMap["LEFT_SHIFT"] = Keyboard::eKey::LEFT_SHIFT;
    _stringMap["LEFT_CONTROL"] = Keyboard::eKey::LEFT_CONTROL;
    _stringMap["LEFT_ALT"] = Keyboard::eKey::LEFT_ALT;
    _stringMap["LEFT_SUPER"] = Keyboard::eKey::LEFT_SUPER;
    _stringMap["RIGHT_SHIFT"] = Keyboard::eKey::RIGHT_SHIFT;
    _stringMap["RIGHT_CONTROL"] = Keyboard::eKey::RIGHT_CONTROL;
    _stringMap["RIGHT_ALT"] = Keyboard::eKey::RIGHT_ALT;
    _stringMap["RIGHT_SUPER"] = Keyboard::eKey::RIGHT_SUPER;
    _stringMap["MENU"] = Keyboard::eKey::MENU;
    _stringMap["LAST"] = Keyboard::eKey::LAST;

    resetKeyboardState();
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
