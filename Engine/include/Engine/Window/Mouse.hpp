#pragma once

#include <unordered_map>

#include <Engine/Window/Cursor.hpp>

struct  sScroll
{
    double  xOffset;
    double  yOffset;
};

class Mouse
{
public:
    enum class      eButton : int
    {
        UNKNOWN = -1,                           // Unknown      button
        MOUSE_BUTTON_1 = 0,                     // Left click   button, according to the mapping below
        MOUSE_BUTTON_2,                         // Middle click button, according to the mapping below
        MOUSE_BUTTON_3,                         // Right click  button, according to the mapping below
        MOUSE_BUTTON_4,                         // ???          button
        MOUSE_BUTTON_5,                         // ???          button
        MOUSE_BUTTON_6,                         // ???          button
        MOUSE_BUTTON_7,                         // ???          button
        MOUSE_BUTTON_8,                         // ???          button
        MOUSE_BUTTON_LAST = MOUSE_BUTTON_8,     // Last         index
        MOUSE_BUTTON_LEFT = MOUSE_BUTTON_1,     // Left click   mapping
        MOUSE_BUTTON_MIDDLE = MOUSE_BUTTON_2,   // Middle click mapping
        MOUSE_BUTTON_RIGHT = MOUSE_BUTTON_3     // Right click  mapping
    }               Button;

    enum class      eButtonState
    {
        CLICK_IDLE = -1,                        // The button is not pressed at all.
        CLICK_PRESSED = 0,                      // The button has just been pressed !
        CLICK_MAINTAINED = 1,                   // The button has been pressed in the previous frame and still is, in the current frame.
        CLICK_RELEASED = 2                      // The button has just been released !
    }               ButtonState;
private:
    typedef std::unordered_map<int, Mouse::eButton>                 MouseNativeMap;
    typedef std::unordered_map<Mouse::eButton, Mouse::eButtonState> MouseStateMap;
public:
    explicit        Mouse();
    virtual         ~Mouse() {}

    MouseNativeMap& getNativeMap();
    MouseStateMap&  getStateMap();
    Cursor&         getCursor();
    sScroll&        getScroll();

    void            resetMouseState();
    void            updateMouseState();

    bool            isPressed(Mouse::eButton button);
private:
    MouseNativeMap  _nativeMap;
    MouseStateMap   _stateMap;

    Cursor          _cursor;
    sScroll         _scroll;
};