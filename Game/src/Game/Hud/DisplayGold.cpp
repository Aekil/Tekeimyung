/*
** @Author : Julien CHARDON
*/

#include <Engine/Core/Components/Components.hh>
#include <Engine/EntityFactory.hpp>

#include <Game/Manager/GameManager.hpp>
#include <Game/Manager/GoldManager.hpp>

#include <Game/Hud/DisplayGold.hpp>

REGISTER_SCRIPT(DisplayGold);

void DisplayGold::start()
{
    this->_em = EntityFactory::getBindedEntityManager();
    if (this->_em != nullptr)
    {
        this->_gameManager = this->_em->getEntityByTag(GAME_MANAGER_TAG);
        if (this->_gameManager != nullptr)
            this->_goldHudDisplay = this->_em->getEntityByTag(HUD_DISPLAY_GOLD_TAG);
    }
}

void DisplayGold::update(float dt)
{
    if (this->_gameManager != nullptr && this->_goldHudDisplay != nullptr)
    {
        sScriptComponent*   scriptComp = this->_gameManager->getComponent<sScriptComponent>();
        GoldManager*        goldManager = scriptComp->getScript<GoldManager>(GOLD_MANAGER_TAG);
        int                 golds = goldManager->getGolds();

        if (golds != this->_tmpGolds)
        {
            sTextComponent*     textComp = _goldHudDisplay->getComponent<sTextComponent>();
            char                goldsText[MAX_SIZE_TEXT_GOLDS];

#if defined(_WIN32)
            sprintf_s(goldsText, "%d", golds);
#else
            sprintf(goldsText, "%d", golds);
#endif

            textComp->text.setContent(goldsText);
            this->_tmpGolds = golds;
        }
    }
}
