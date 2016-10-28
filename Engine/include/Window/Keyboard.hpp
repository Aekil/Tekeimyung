#pragma once

#include <unordered_map>
#include <string>

class Keyboard
{
public:
	enum class          eKey : int
	{
		UNKNOWN = -1,	// Unknown	    key
        SPACE = 0,      // Space        key
        APOSTROPHE,     // "            key
        COMMA,          // ,            key
        MINUS,          // Main -       key
        PERIOD,         // >            key
        SLASH,          // /            key
        KEY_0,          // Main 0       key
        KEY_1,          // Main 1       key
        KEY_2,          // Main 2       key
        KEY_3,          // Main 3       key
        KEY_4,          // Main 4       key
        KEY_5,          // Main 5       key
        KEY_6,          // Main 6       key
        KEY_7,          // Main 7       key
        KEY_8,          // Main 8       key
        KEY_9,          // Main 9       key
        SEMICOLON,      // ;            key
        EQUAL,          // =            key
        A,              // A            key
        B,              // B            key
        C,              // C            key
        D,              // D            key
        E,              // E            key
        F,              // F            key
        G,              // G            key
        H,              // H            key
        I,              // I            key
        J,              // J            key
        K,              // K            key
        L,              // L            key
        M,              // M            key
        N,              // N            key
        O,              // O            key
        P,              // P            key
        Q,              // Q            key
        R,              // R            key
        S,              // S            key
        T,              // T            key
        U,              // U            key
        V,              // V            key
        W,              // W            key
        X,              // X            key
        Y,              // Y            key
        Z,              // Z            key
        LEFT_BRACKET,   // {            key
        BACKSLASH,      // \            key
        RIGHT_BRACKET,  // }            key
        GRAVE_ACCENT,   // `            key
        WORLD_1,        // ???          key
        WORLD_2,        // ???          key
        ESCAPE,         // Esc.         key
        ENTER,          // Enter        key
        TAB,            // Tabulation   key
        BACKSPACE,      // Backspace    key
        INSERT,         // Insert       key
        DELETE,         // Delete       key
        RIGHT,          // Right arrow  key
        LEFT,           // Left arrow   key
        DOWN,           // Down arrow   key
        UP,             // Up arrow     key
        PAGE_UP,        // Page up      key
        PAGE_DOWN,      // Page down    key
        HOME,           // ???          key
        END,            // End          key
        CAPS_LOCK,      // Caps lock    key
        SCROLL_LOCK,    // Scroll lock  key
        NUM_LOCK,       // Num. lock    key
        PRINT_SCREEN,   // Print screen key
        PAUSE,          // Pause        key
        F1,             // F1           key
        F2,             // F2           key
        F3,             // F3           key
        F4,             // F4           key
        F5,             // F5           key
        F6,             // F6           key
        F7,             // F7           key
        F8,             // F8           key
        F9,             // F9           key
        F10,            // F10          key
        F11,            // F11          key
        F12,            // F12          key
        F13,            // F13          key
        F14,            // F14          key
        F15,            // F15          key
        F16,            // F16          key
        F17,            // F17          key
        F18,            // F18          key
        F19,            // F19          key
        F20,            // F20          key
        F21,            // F21          key
        F22,            // F22          key
        F23,            // F23          key
        F24,            // F24          key
        F25,            // F25          key
        KP_0,           // Keypad 0     key
        KP_1,           // Keypad 1     key
        KP_2,           // Keypad 2     key
        KP_3,           // Keypad 3     key
        KP_4,           // Keypad 4     key
        KP_5,           // Keypad 5     key
        KP_6,           // Keypad 6     key
        KP_7,           // Keypad 7     key
        KP_8,           // Keypad 8     key
        KP_9,           // Keypad 9     key
        KP_DECIMAL,     // Keypad .     key
        KP_DIVIDE,      // Keypad /     key
        KP_MULTIPLY,    // Keypad *     key
        KP_SUBTRACT,    // Keypad -     key
        KP_ADD,         // Keypad +     key
        KP_ENTER,       // Keypad enter key
        KP_EQUAL,       // Keypad =     key
        LEFT_SHIFT,     // Left shift   key
        LEFT_CONTROL,   // Left ctrl    key
        LEFT_ALT,       // Left alt     key
        LEFT_SUPER,     // ???          key
        RIGHT_SHIFT,    // Right shift  key
        RIGHT_CONTROL,  // Right ctrl   key
        RIGHT_ALT,      // Right alt    key
        RIGHT_SUPER,    // ???          key
        MENU,           // ???          key
        LAST = MENU     // Last         index
	}                   Key;

	enum class          eKeyState
	{
        KEY_IDLE = -1,      // The key is not pressed at all.
		KEY_PRESSED = 0,	// The key has just been pressed !
		KEY_MAINTAINED,		// The key has been pressed in the previous frame and still is, in the current frame.
		KEY_RELEASED		// The key has just been released !
	}                   KeyState;
private:
	typedef std::unordered_map<int, Keyboard::eKey>                 KeyboardNativeMap;
    typedef std::unordered_map<Keyboard::eKey, Keyboard::eKeyState> KeyboardStateMap;
    typedef std::unordered_map<std::string, Keyboard::eKey>    KeyboardStringMap;
public:
	explicit            Keyboard();
	virtual             ~Keyboard() {}

	KeyboardNativeMap&  getNativeMap();
    KeyboardStateMap&   getStateMap();
    KeyboardStringMap&  getStringMap();
    std::string         keyToString(Keyboard::eKey key);
    Keyboard::eKeyState operator[](Keyboard::eKey key);

    void                resetKeyboardState();
    void                updateKeyboardState();

    bool                isPressed(Keyboard::eKey key);
private:
	KeyboardNativeMap   _nativeMap;
    KeyboardStateMap    _stateMap;
    KeyboardStringMap   _stringMap;
};
