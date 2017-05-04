/**
* @Author   Julien CHARDON
*/

#include <Engine/EntityFactory.hpp>
#include <Game/Scripts/GameManager.hpp>
#include <Game/Scripts/TutoManager.hpp>
#include <Game/Scripts/WaveManager.hpp>
#include <Game/Scripts/GoldManager.hpp>
#include <Game/Scripts/TutoManagerMessage.hpp>

TutoManager::~TutoManager()
{
    TutoManagerMessage::_instance = nullptr;
}

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
    _statesMessages[eTutoState::MOVE] = "Press W, A, S or D to move (Press K to SKIP the tutorial)";
    _statesMessages[eTutoState::SHOOT] = "Use LEFT CLICK to shoot !";
    _statesMessages[eTutoState::CHANGE_WEAPON] = "Use the mouse SCROLL WHEEL to change your weapon !";
    _statesMessages[eTutoState::CHOOSE_BUILD] = "Press 1, 2, 3, 4 or 5 to choose an item to build !";
    _statesMessages[eTutoState::BUILD] = "Use LEFT CLICK on buildable zone to build something";
    _statesMessages[eTutoState::DISABLE_BUILD] = "Use RIGHT CLICK to disable build mode";
    _statesMessages[eTutoState::CHECK_HOWTOPLAY] = "Try to check the 'How to Play' from the pause menu (press Escape)";
    _statesMessages[eTutoState::CHECK_BUILDLIST] = "Check the Build list with B to know what you can build and for how much golds";
    _statesMessages[eTutoState::TUTO_DONE] = "Well done ! Tutorial completed. GL & HF ! (press T to QUIT tutorial)";
    _statesMessages[eTutoState::TUTO_WAVE] = "Press SPACE to start the wave";
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
    if (state == this->_currentState && this->_textComp != nullptr)
    {
        ++this->_currentState;
        this->_textComp->text.setContent(this->_statesMessages[this->_currentState]);

        if (state == eTutoState::TUTO_DONE)
            this->_waveManager->setTutorialIsFinished(true);

        if (this->_currentState == eTutoState::BUILD)
            this->_goldManager->setGolds(50);
        if (this->_currentState == eTutoState::DISABLE_BUILD)
            this->_goldManager->setGolds(0);
    }
}

/**
    This is a static function called from WaveManager,
    that is why getEntityByTag is used !
*/
void        TutoManager::display(bool displayed)
{
    auto    em = EntityFactory::getBindedEntityManager();
    Entity* tutoManager = em->getEntityByTag("TutoManager");

    if (tutoManager != nullptr)
    {
        sRenderComponent*   render = tutoManager->getComponent<sRenderComponent>();

        if (render != nullptr)
            render->enabled = displayed;
    }
}
