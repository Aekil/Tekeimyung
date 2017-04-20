/**
* @Author   Julien CHARDON
*/

#include <Engine/EntityFactory.hpp>
#include <Game/Scripts/GameManager.hpp>
#include <Game/Scripts/TutoManager.hpp>

eTutoState& operator++(eTutoState& state)
{
    int tmp = static_cast<int>(state);
    ++tmp;
    state = static_cast<eTutoState>(tmp);
    return (state);
}

eTutoState operator++(eTutoState& state, int)
{
    eTutoState tmp(state);
    ++state;
    return (tmp);
}

TutoManager::TutoManager()
{
    _statesMessages[eTutoState::MOVE] = "Use W,A,S,D to move";
    _statesMessages[eTutoState::SHOOT] = "Use left click to shoot !";
    _statesMessages[eTutoState::CHANGE_WEAPON] = "Use the scroll wheel to change your weapon (you have 3 different) !";
    _statesMessages[eTutoState::CHOOSE_BUILD] = "Use keys from 1 to 4 (on the top of your keyboard)\n    to choose a building and enable build zone!";
    _statesMessages[eTutoState::BUILD] = "Use Left Click on buildable zone to build something";
    _statesMessages[eTutoState::DISABLE_BUILD] = "Use Right Click to disable build mode";
    _statesMessages[eTutoState::CHECK_HOWTOPLAY] = "Check the HowToPlay as reminder with H\n\n(remember that you can always check it again with the same shortcut)";
    _statesMessages[eTutoState::CHECK_BUILDLIST] = "Check the Build list with B to know what you can build and for how much golds.\n\n(remember that you can always check it again with the same shortcut)";
    _statesMessages[eTutoState::TUTO_DONE] = "Well done ! Tutorial completed.\n\nGood luck & Have fun !";
    _statesMessages[eTutoState::TUTO_HIDDEN] = "";
}

void TutoManager::start()
{
    auto em = EntityFactory::getBindedEntityManager();
    Entity* tutoDisplay = em->getEntityByTag(TUTO_MANAGER_TAG);
    if (tutoDisplay)
    {
        _textComp = tutoDisplay->getComponent<sTextComponent>();
    }
}

void TutoManager::update(float dt) {}

void    TutoManager::sendMessage(eTutoState state)
{
    if (state == _currentState && _textComp)
    {
        ++_currentState;
        _textComp->text.setContent(_statesMessages[_currentState]);
    }
}
