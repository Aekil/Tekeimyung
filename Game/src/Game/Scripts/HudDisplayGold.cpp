/*
** @Author : Julien CHARDON
*/

#include <Engine/EntityFactory.hpp>

#include <Game/Scripts/HudDisplayGold.hpp>

void HudDisplayGold::start()
{
}

void HudDisplayGold::update(float dt)
{
    static int ngold = 0;
    /*static float elapsedTime = 0;

    elapsedTime += dt;
    if (elapsedTime >= GOLD_TIMER)
    {
        
        elapsedTime -= GOLD_TIMER;
    }*/
    ngold += 1;
    EntityManager* em = EntityFactory::getBindedEntityManager();

    const auto& goldDisplay = em->getEntityByTag("HudDisplayGold");
    /*const auto& gameManager = em->getEntityByTag("GameManager");
    sScriptComponent* scriptComp;
    scriptComp = gameManager->getComponent<sScriptComponent>();
    scriptComp->*/

    sTextComponent* textComp;
    textComp = goldDisplay->getComponent<sTextComponent>();
    char golds[20] = { 0 };
    sprintf_s(golds, "%d Golds", ngold);
    textComp->text.setContent(golds);
}
