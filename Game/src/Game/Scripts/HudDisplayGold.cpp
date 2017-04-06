/*
** @Author : Julien CHARDON
*/

#include <Engine/EntityFactory.hpp>

#include <Game/Scripts/GameManager.hpp>
#include <Game/Scripts/GoldManager.hpp>

#include <Game/Scripts/HudDisplayGold.hpp>

void HudDisplayGold::start()
{
    _em = EntityFactory::getBindedEntityManager();
    if (_em != nullptr)
    {
        _gameManager = _em->getEntityByTag(GAME_MANAGER_TAG);
        if (_gameManager != nullptr)
            _goldHudDisplay = _em->getEntityByTag(HUD_DISPLAY_GOLD_TAG);
    }
}

void HudDisplayGold::update(float dt)
{
    if (_gameManager != nullptr && _goldHudDisplay != nullptr)
    {
        sScriptComponent*   scriptComp = _gameManager->getComponent<sScriptComponent>();
        GoldManager*        goldManager = scriptComp->getScript<GoldManager>(GOLD_MANAGER_TAG);
        sTextComponent*     textComp = _goldHudDisplay->getComponent<sTextComponent>();
        char                goldsText[MAX_SIZE_TEXT_GOLDS];
        int                 golds = goldManager->getGolds();

        sprintf_s(goldsText, "%d Golds", golds);
        textComp->text.setContent(goldsText);
    }
}
