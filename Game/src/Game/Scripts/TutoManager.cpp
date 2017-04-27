/**
* @Author   Julien CHARDON
*/

#include <Engine/EntityFactory.hpp>
#include <Game/Scripts/GameManager.hpp>
#include <Game/Scripts/TutoManager.hpp>
#include <Game/Scripts/WaveManager.hpp>
#include <Game/Scripts/GoldManager.hpp>

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
    _statesMessages[eTutoState::MOVE] = "Use W,A,S,D to move (Press K to skip the tutorial)";
    _statesMessages[eTutoState::SHOOT] = "Use left click to shoot !";
    _statesMessages[eTutoState::CHANGE_WEAPON] = "Use the scroll wheel to change your weapon (you have 2 different) !";
    _statesMessages[eTutoState::CHOOSE_BUILD] = "Use keys from 1 to 4 (on the top of your keyboard)\n    to choose a building and enable build zone!";
    _statesMessages[eTutoState::BUILD] = "Use Left Click on buildable zone to build something";
    _statesMessages[eTutoState::DISABLE_BUILD] = "Use Right Click to disable build mode";
    _statesMessages[eTutoState::CHECK_HOWTOPLAY] = "Check the HowToPlay as reminder with H\n\n(remember that you can always check it again with the same shortcut)";
    _statesMessages[eTutoState::CHECK_BUILDLIST] = "Check the Build list with B to know what you can build and for how much golds.\n\n(remember that you can always check it again with the same shortcut)";
    _statesMessages[eTutoState::TUTO_DONE] = "Well done ! Tutorial completed.\n\nGood luck & Have fun ! (press T to hide tutorial and begin the game)";
    _statesMessages[eTutoState::TUTO_HIDDEN] = "";
}

void TutoManager::start()
{
    this->_textComp = this->getComponent<sTextComponent>();

    auto em = EntityFactory::getBindedEntityManager();

    auto entity = em->getEntityByTag("GameManager");

    if (entity == nullptr)
    {
        LOG_ERROR("No entity with GameManager tag");
        return;
    }

    auto gameManagerScriptComponent = entity->getComponent<sScriptComponent>();

    if (gameManagerScriptComponent == nullptr)
    {
        LOG_ERROR("No scriptComponent on entity");
        return;
    }

    this->_waveManager = gameManagerScriptComponent->getScript<WaveManager>("WaveManager");

    if (this->_waveManager == nullptr)
    {
        LOG_ERROR("No WaveManager script on entity");
        return;
    }

    this->_goldManager = gameManagerScriptComponent->getScript<GoldManager>("GoldManager");

    if (this->_goldManager == nullptr)
    {
        LOG_ERROR("No GoldManager script on entity");
        return;
    }

    this->_goldManager->setIncreaseOnTime(false);
    this->_goldManager->setGolds(0);
}

void TutoManager::update(float dt)
{
    if (this->keyboard.getStateMap()[Keyboard::eKey::K] == Keyboard::eKeyState::KEY_RELEASED)
    {
        this->_currentState = eTutoState::TUTO_DONE;
        this->sendMessage(eTutoState::TUTO_DONE);
    }
}

void    TutoManager::sendMessage(eTutoState state)
{
    if (state == _currentState && _textComp)
    {
        ++_currentState;
        _textComp->text.setContent(_statesMessages[_currentState]);
        if (state == eTutoState::TUTO_DONE)
        {
            this->_waveManager->setTutorialIsFinished(true);
            this->_goldManager->setIncreaseOnTime(true);
            this->_goldManager->setGolds(STARTING_AMOUNT_OF_GOLD);

            this->Destroy();
        }

        if (this->_currentState == eTutoState::BUILD)
            this->_goldManager->setGolds(50);

        if (this->_currentState == eTutoState::DISABLE_BUILD)
            this->_goldManager->setGolds(0);
    }
}
