#include <Game/Scripts/TutoManagerMessage.hpp>
#include <Engine/EntityFactory.hpp>

std::unique_ptr<TutoManagerMessage>     TutoManagerMessage::_instance = nullptr;

TutoManagerMessage* TutoManagerMessage::getInstance()
{
    if (!_instance)
    {
        _instance = std::make_unique<TutoManagerMessage>();
        _instance->init();
    }

    return (_instance.get());
}

void    TutoManagerMessage::init()
{
    // Get TutoManager
    {
        auto em = EntityFactory::getBindedEntityManager();
        Entity* tutoManager = em->getEntityByTag("TutoManager");
        if (!tutoManager)
        {
            LOG_WARN("Can't find entity with TutoManager tag");
            return;
        }

        auto scriptComponent = tutoManager->getComponent<sScriptComponent>();

        if (!scriptComponent)
        {
            LOG_WARN("Can't find scriptComponent on TutoManager entity");
            return;
        }

        _tutoManager = scriptComponent->getScript<TutoManager>("TutoManager");
    }

}

void    TutoManagerMessage::sendMessage(eTutoState state)
{
    if (!_tutoManager)
        return;

    _tutoManager->sendMessage(state);
}

bool    TutoManagerMessage::stateOnGoingOrDone(eTutoState state)
{
    if (!_tutoManager)
        return (true);

    return (_tutoManager->stateOnGoingOrDone(state));
}

bool    TutoManagerMessage::stateOnGoing(eTutoState state)
{
    if (!_tutoManager)
        return (false);

    return (_tutoManager->stateOnGoing(state));
}

bool    TutoManagerMessage::tutorialDone()
{
    if (!_tutoManager)
        return (true);

    return (_tutoManager->tutorialDone());
}
