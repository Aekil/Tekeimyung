/*
** @Author : Julien CHARDON
*/

#include <Engine/EntityFactory.hpp>

#include <Game/Scripts/GoldManager.hpp>

#include <Game/Scripts/HudDisplayGold.hpp>

void HudDisplayGold::start()
{
}

void HudDisplayGold::update(float dt)
{
    /*static float elapsedTime = 0;

    elapsedTime += dt;
    if (elapsedTime >= GOLD_TIMER)
    {
        
        elapsedTime -= GOLD_TIMER;
    }*/
    EntityManager* em = EntityFactory::getBindedEntityManager();

    const auto& goldDisplay = em->getEntityByTag("HudDisplayGold");
    const auto& gameManager = em->getEntityByTag("GameManager");
    sScriptComponent* scriptComp;
    scriptComp = gameManager->getComponent<sScriptComponent>();
    GoldManager* goldManager = scriptComp->getScript<GoldManager>("GoldManager");
    int golds = goldManager->getGolds();

    sTextComponent* textComp;
    textComp = goldDisplay->getComponent<sTextComponent>();
    char goldsText[20] = { 0 };
    sprintf_s(goldsText, "%d Golds", golds);
    textComp->text.setContent(goldsText);
}
