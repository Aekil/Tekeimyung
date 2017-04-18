/**
* @Author   Julien CHARDON
*/

#include <Engine/EntityFactory.hpp>
#include <Game/Scripts/GameManager.hpp>
#include <Game/Scripts/TutoManager.hpp>

void TutoManager::start()
{
    _em = EntityFactory::getBindedEntityManager();
    if (_em != nullptr)
    {
        _gameManager = _em->getEntityByTag(GAME_MANAGER_TAG);
        if (_gameManager != nullptr)
            _tutoDisplay = _em->getEntityByTag(TUTO_MANAGER_TAG);
    }
    _states = MOVE;
}

void TutoManager::update(float dt)
{
    if (_gameManager != nullptr && _tutoDisplay != nullptr)
    {
        sScriptComponent*   scriptComp = _gameManager->getComponent<sScriptComponent>();
        TutoManager*        tutoManager = scriptComp->getScript<TutoManager>(TUTO_MANAGER_TAG);
        sTextComponent*     textComp = _tutoDisplay->getComponent<sTextComponent>();

        auto &&keyboard = GameWindow::getInstance()->getKeyboard();
        auto &&mouse = GameWindow::getInstance()->getMouse();
        //char                goldsText[30];

        //sprintf_s(goldsText, "%d Golds", golds);
        switch (_states)
        {
        case MOVE:
            textComp->text.setContent("Use W,A,S,D to move");
            if (keyboard.getStateMap()[Keyboard::eKey::S] == Keyboard::eKeyState::KEY_PRESSED)
                _states = ENABLE_BUILD;
            break;
        case ENABLE_BUILD:
            textComp->text.setContent("Use E to enable build zone");
            if (keyboard.getStateMap()[Keyboard::eKey::E] == Keyboard::eKeyState::KEY_PRESSED)
                _states = BUILD;
            break;
        case BUILD:
            textComp->text.setContent("Use Left Click on buildable zone to build something");
            if (mouse.getStateMap()[Mouse::eButton::MOUSE_BUTTON_1] == Mouse::eButtonState::CLICK_PRESSED)
                _states = TUTO_DONE;
            break;
        case TUTO_DONE:
            textComp->text.setContent("Well done ! Tutorial completed.");
            break;
        default:
            break;
        }
    }
}