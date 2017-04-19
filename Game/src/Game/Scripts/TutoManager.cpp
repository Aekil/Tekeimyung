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
        auto &&scroll = mouse.getScroll();
        static int shootCounter = 0;

        switch (_states)
        {
        case MOVE:
            textComp->text.setContent("Use W,A,S,D to move");
            if (keyboard.getStateMap()[Keyboard::eKey::S] == Keyboard::eKeyState::KEY_PRESSED) // improve this test ?
                _states = SHOOT;
            break;
        case SHOOT:
            textComp->text.setContent("Use left click to shoot !");
            if (mouse.getStateMap()[Mouse::eButton::MOUSE_BUTTON_1] == Mouse::eButtonState::CLICK_PRESSED)
                _states = CHANGE_WEAPON;
            break;
        case CHANGE_WEAPON:
            static double lastOffset = scroll.yOffset;
            textComp->text.setContent("Use the scroll wheel to change your weapon (you have 3 different) !");
            if (lastOffset != scroll.yOffset)
                _states = CHOOSE_BUILD;
            break;
        case CHOOSE_BUILD:
            textComp->text.setContent("Use keys from 1 to 5 (on the top of your keyboard)\n    to choose a building and enable build zone!");
            if (keyboard.getStateMap()[Keyboard::eKey::KEY_1] == Keyboard::eKeyState::KEY_PRESSED ||
                keyboard.getStateMap()[Keyboard::eKey::KEY_2] == Keyboard::eKeyState::KEY_PRESSED ||
                keyboard.getStateMap()[Keyboard::eKey::KEY_3] == Keyboard::eKeyState::KEY_PRESSED || 
                keyboard.getStateMap()[Keyboard::eKey::KEY_4] == Keyboard::eKeyState::KEY_PRESSED || 
                keyboard.getStateMap()[Keyboard::eKey::KEY_5] == Keyboard::eKeyState::KEY_PRESSED)
                _states = BUILD;
            break;
        case BUILD:
            textComp->text.setContent("Use Left Click on buildable zone to build something");
            if (mouse.getStateMap()[Mouse::eButton::MOUSE_BUTTON_1] == Mouse::eButtonState::CLICK_PRESSED) // improve this test !
                _states = DISABLE_BUILD;
            break;
        case DISABLE_BUILD:
            textComp->text.setContent("Use Right Click to disable build mode");
            if (mouse.getStateMap()[Mouse::eButton::MOUSE_BUTTON_2] == Mouse::eButtonState::CLICK_PRESSED)
                _states = CHECK_HOWTOPLAY;
            break;
        case CHECK_HOWTOPLAY:
            textComp->text.setContent("Check the HowToPlay as reminder with H\n\n(remember that you can always check it again with the same shortcut)");
            if (keyboard.getStateMap()[Keyboard::eKey::H] == Keyboard::eKeyState::KEY_PRESSED)
                _states = CHECK_BUILDLIST;
            break;
        case CHECK_BUILDLIST:
            textComp->text.setContent("Check the Build list with B to know what you can build and for how much golds.\n\n(remember that you can always check it again with the same shortcut)");
            if (keyboard.getStateMap()[Keyboard::eKey::B] == Keyboard::eKeyState::KEY_PRESSED)
                _states = TUTO_DONE;
            break;
        case TUTO_DONE:
            textComp->text.setContent("Well done ! Tutorial completed.\n\nGood luck & Have fun !");
            if (mouse.getStateMap()[Mouse::eButton::MOUSE_BUTTON_1] == Mouse::eButtonState::CLICK_PRESSED)
                shootCounter += 1;
            if (shootCounter >= 5)
                _states = TUTO_HIDDEN;
            break;
        case TUTO_HIDDEN:
            textComp->text.setContent("");
            break;
        default:
            break;
        }
    }
}