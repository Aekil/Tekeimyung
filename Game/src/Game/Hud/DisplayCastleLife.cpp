/*
** @Author : Julien CHARDON
*/

#include <Engine/Core/Components/Components.hh>
#include <Engine/EntityFactory.hpp>

#include <Game/Manager/GameManager.hpp>
#include <Game/Building/Castle.hpp>

#include <Game/Hud/DisplayCastleLife.hpp>

REGISTER_SCRIPT(DisplayCastleLife);

void DisplayCastleLife::start()
{
    this->_em = EntityFactory::getBindedEntityManager();
    if (this->_em != nullptr)
    {
        this->_player = this->_em->getEntityByTag(CASTLE_TAG);
        if (this->_player != nullptr)
            this->_castleLifeHudDisplay = this->_em->getEntityByTag(HUD_DISPLAY_CASTLELIFE_TAG);
    }
}

void DisplayCastleLife::update(float dt)
{
    if (this->_player != nullptr && this->_castleLifeHudDisplay != nullptr)
    {
        sScriptComponent*   scriptComp = this->_player->getComponent<sScriptComponent>();

        if (scriptComp == nullptr)
            return;

        Castle*             castle = scriptComp->getScript<Castle>(CASTLE_TAG);

        if (castle == nullptr)
            return;

        int                 castleLife = castle->getHealth();
        int                 maxCastleLife = castle->getMaxHealth();

        if (castleLife != this->_tmpCastleLife || maxCastleLife != this->_tmpMaxCastleLife)
        {
            sTextComponent*     textComp = _castleLifeHudDisplay->getComponent<sTextComponent>();
            char                castleLifeText[MAX_SIZE_TEXT_CASTLELIFE];

#if defined(_WIN32)
            sprintf_s(castleLifeText, "%d/%d", castleLife, maxCastleLife);
#else
            sprintf(castleLifeText, "%d/%d", castleLife, maxCastleLife);
#endif

            textComp->text.setContent(castleLifeText);
            this->_tmpCastleLife = castleLife;
            this->_tmpMaxCastleLife = maxCastleLife;
        }
    }
}
