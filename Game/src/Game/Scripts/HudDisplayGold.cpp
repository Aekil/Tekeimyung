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
    // update every frame, unless we can call update after every gold transaction
    EntityManager*  em = EntityFactory::getBindedEntityManager();
    const auto&     gameManager = em->getEntityByTag("GameManager");

    if (em != nullptr && gameManager != nullptr)
    {
        const auto&         goldDisplay = em->getEntityByTag("HudDisplayGold");

        if (goldDisplay != nullptr)
        {
            sScriptComponent*   scriptComp = gameManager->getComponent<sScriptComponent>();
            GoldManager*        goldManager = scriptComp->getScript<GoldManager>("GoldManager");
            int                 golds = goldManager->getGolds();

            sTextComponent* textComp = goldDisplay->getComponent<sTextComponent>();
            char goldsText[20];
            sprintf_s(goldsText, "%dGolds", golds);
            textComp->text.setContent(goldsText);
        }

    }
}
