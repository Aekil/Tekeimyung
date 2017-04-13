/*
** @Author : Julien CHARDON
*/

#include <Engine/EntityFactory.hpp>

#include <Game/Scripts/GameManager.hpp>
#include <Game/Scripts/Castle.hpp>

#include <Game/Scripts/HudDisplayCastleLife.hpp>

void HudDisplayCastleLife::start()
{
    _em = EntityFactory::getBindedEntityManager();
    if (_em != nullptr)
    {
        _castle = _em->getEntityByTag(CASTLE_TAG);
        if (_castle != nullptr)
            _castleLifeHudDisplay = _em->getEntityByTag(HUD_DISPLAY_CASTLELIFE_TAG);
    }
}

void HudDisplayCastleLife::update(float dt)
{
    if (_castle != nullptr && _castleLifeHudDisplay != nullptr)
    {
        sScriptComponent*   scriptComp = _castle->getComponent<sScriptComponent>();
        Castle*             castle = scriptComp->getScript<Castle>(CASTLE_TAG);
        int                 castleLife = castle->getHealth();
        sTextComponent*     textComp = _castleLifeHudDisplay->getComponent<sTextComponent>();
        char                castleLifeText[MAX_SIZE_TEXT_CASTLELIFE];

        sprintf_s(castleLifeText, "Castle Health :\n%d/%d", castleLife, MAX_CASTLE_HEALTH);
        textComp->text.setContent(castleLifeText);
    }
}