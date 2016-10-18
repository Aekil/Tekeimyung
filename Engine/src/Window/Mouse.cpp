#include <GLFW/glfw3.h>

#include <Window/Mouse.hpp>

Mouse::Mouse()
{
    _nativeMap[GLFW_MOUSE_BUTTON_1] = Mouse::eButton::MOUSE_BUTTON_1;
    _nativeMap[GLFW_MOUSE_BUTTON_2] = Mouse::eButton::MOUSE_BUTTON_2;
    _nativeMap[GLFW_MOUSE_BUTTON_3] = Mouse::eButton::MOUSE_BUTTON_3;
    _nativeMap[GLFW_MOUSE_BUTTON_4] = Mouse::eButton::MOUSE_BUTTON_4;
    _nativeMap[GLFW_MOUSE_BUTTON_5] = Mouse::eButton::MOUSE_BUTTON_5;
    _nativeMap[GLFW_MOUSE_BUTTON_6] = Mouse::eButton::MOUSE_BUTTON_6;
    _nativeMap[GLFW_MOUSE_BUTTON_7] = Mouse::eButton::MOUSE_BUTTON_7;
    _nativeMap[GLFW_MOUSE_BUTTON_8] = Mouse::eButton::MOUSE_BUTTON_8;

    resetMouseState();
}

Mouse::MouseNativeMap&  Mouse::getNativeMap()
{
    return (_nativeMap);
}

Mouse::MouseStateMap&   Mouse::getStateMap()
{
    return (_stateMap);
}

Cursor&     Mouse::getCursor()
{
    return (_cursor);
}

void    Mouse::resetMouseState()
{
    Mouse::MouseNativeMap::const_iterator   it;

    for (it = _nativeMap.begin(); it != _nativeMap.end(); ++it)
    {
        _stateMap[it->second] = Mouse::eButtonState::CLICK_IDLE;
    }
}

void    Mouse::updateMouseState()
{
    Mouse::MouseNativeMap::const_iterator   it;

    for (it = _nativeMap.begin(); it != _nativeMap.end(); ++it)
    {
        switch (_stateMap[it->second])
        {
            case Mouse::eButtonState::CLICK_RELEASED:
                _stateMap[it->second] = Mouse::eButtonState::CLICK_IDLE;
                break;
            case Mouse::eButtonState::CLICK_PRESSED:
                _stateMap[it->second] = Mouse::eButtonState::CLICK_MAINTAINED;
                break;
            default:
                break;
        }
    }
}

bool    Mouse::isPressed(Mouse::eButton button)
{
    return (_stateMap[button] == Mouse::eButtonState::CLICK_PRESSED ||
            _stateMap[button] == Mouse::eButtonState::CLICK_MAINTAINED);
}