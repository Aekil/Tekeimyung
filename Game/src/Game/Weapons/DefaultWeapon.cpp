/*
** Author : Simon AMBROISE
*/

#include <Engine/EntityFactory.hpp>

#include <Game/Weapons/DefaultWeapon.hpp>
#include <Game/Scripts/Projectile.hpp>

DefaultWeapon::DefaultWeapon()
{
    this->_attributes["FireRate"] = new Attribute(1.0f);
    this->_attributes["CurrentAmmo"] = new Attribute(100.0f);
    this->_attributes["Damage"] = new Attribute(45.0f);

    _shootSound = EventSound::getEventByEventType(eEventSound::PLAYER_SHOOT);
}

void DefaultWeapon::fire(sTransformComponent* playerTransform, sRenderComponent* playerRender, glm::vec3 playerDirection)
{
    Entity*                 bullet;
    sScriptComponent*       bulletScripts;
    Projectile*             projectileScript;

    this->_attributes["CurrentAmmo"]->addBonus(Bonus(-1));

    if (this->_attributes["CurrentAmmo"]->getFinalValue() == 0)
        this->reload();

    bullet = EntityFactory::createEntity("PLAYER_BULLET");
    bulletScripts = bullet->getComponent<sScriptComponent>();
    projectileScript = bulletScripts->getScript<Projectile>("Projectile");

    projectileScript->_projectileTransform->setPos(playerTransform->getPos());

    projectileScript->_projectileTransform->translate(glm::vec3(0.0f, -((playerRender->getModel()->getMin().y * playerTransform->getScale().y) / 2.0f), 0.0f));

    projectileScript->_damage = this->_attributes["Damage"]->getFinalValue();
    projectileScript->followDirection({ playerDirection.x, 0.0f, playerDirection.z });

#if (ENABLE_SOUND)
    if (_shootSound->soundID != -1 && !SoundManager::getInstance()->isSoundPlaying(_shootSound->soundID))
    {
        SoundManager::getInstance()->playSound(_shootSound->soundID);
    }
#endif
}

void DefaultWeapon::reload()
{
    this->_attributes["CurrentAmmo"]->clearAllBonuses();
}