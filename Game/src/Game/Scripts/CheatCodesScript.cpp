/*
** @Author :Guillaume Labey
*/

#include <Engine/EntityFactory.hpp>
#include <Engine/Core/Components/Components.hh>
#include <Engine/Window/GameWindow.hpp>

#include <Game/Scripts/CheatCodesScript.hpp>

REGISTER_SCRIPT(CheatCodesScript);

void CheatCodesScript::start()
{
    _textComp = entity->getComponent<sTextComponent>();
}

void CheatCodesScript::update(float dt)
{
    auto gameWindow = GameWindow::getInstance();
    auto &keyboard = gameWindow->getKeyboard();
    static float deleteMaintainedInterval = 0.1f;

    // We don't want the float to overflow
    if (_lastDelete < deleteMaintainedInterval)
    {
        _lastDelete += dt;
    }

    if (_textComp->text.getContent().size() != 0 &&
        (keyboard.getStateMap()[Keyboard::eKey::BACKSPACE] == Keyboard::eKeyState::KEY_PRESSED ||
        (keyboard.getStateMap()[Keyboard::eKey::BACKSPACE] == Keyboard::eKeyState::KEY_MAINTAINED &&
            _lastDelete >= deleteMaintainedInterval)))
    {
        std::string textContent = _textComp->text.getContent();
        textContent.erase(textContent.end() - 1, textContent.end());

        _textComp->text.setContent(textContent);
        _lastDelete = 0.0f;
    }
    else
    {
        _textComp->text.addContent(keyboard.getTypedText());
    }
}

const Text& CheatCodesScript::getText() const
{
    return (_textComp->text);
}
