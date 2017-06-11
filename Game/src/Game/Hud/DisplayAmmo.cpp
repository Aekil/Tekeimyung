/*
** @Author : Julien CHARDON
*/

#include <string>

#include <Engine/Core/Components/Components.hh>
#include <Engine/EntityFactory.hpp>
#include <Engine/Utils/Exception.hpp>

#include <Game/Manager/GameManager.hpp>
#include <Game/Character/Player.hpp>
#include <Game/Weapons/IWeapon.hpp>
#include <Game/Attibutes/Attribute.hpp>

#include <Game/Hud/DisplayAmmo.hpp>

REGISTER_SCRIPT(DisplayAmmo);

void DisplayAmmo::start()
{
    this->_em = EntityFactory::getBindedEntityManager();
    if (this->_em != nullptr)
    {
        this->_player = this->_em->getEntityByTag("Player");
        if (this->_player == nullptr)
            EXCEPT(NullptrException, "No entity with tag %s", "Player");
    }
}

void DisplayAmmo::update(float dt)
{
    sScriptComponent*   scriptComp = this->_player->getComponent<sScriptComponent>();

    if (scriptComp == nullptr)
        EXCEPT(NullptrException, "Component %s not found on entity with tag %s", "sScriptComponent", "Player");

    Player*             player = scriptComp->getScript<Player>("Player");

    if (player == nullptr)
        EXCEPT(NullptrException, "Script %s not found on entity with tag %s", "Player", "Player");

    IWeapon*      weapon = player->getActualWeapon();

    auto maxAmmo = weapon->_attributes["Ammo"]->getBaseValue();
    auto actualAmmo = weapon->_attributes["Ammo"]->getFinalValue();

    sTextComponent*     textComp = this->getComponent<sTextComponent>();

    if (textComp == nullptr)
        EXCEPT(NullptrException, "Component %s not found on entity with tag %s", "sTextComponent", "HUD_DISPLAY_AMMO");

    if (this->_previousAmmo != actualAmmo)
    {
        char                ammoText[MAX_SIZE_TEXT_AMMO];

#if defined(_WIN32)
        sprintf_s(ammoText, "%d/%d", (int)actualAmmo, (int)maxAmmo);
#else
        sprintf(ammoText, "%d/%d", (int)actualAmmo, (int)maxAmmo);
#endif

        textComp->text.setContent(ammoText);
        this->_previousAmmo = actualAmmo;
    }
}
