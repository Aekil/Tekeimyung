/*
** Author : Simon AMBROISE
*/

#include <Engine/EntityFactory.hpp>
#include <Engine/Utils/ResourceManager.hpp>
#include <Engine/Utils/Maths.hpp>

#include <Game/Weapons/DefaultWeapon.hpp>
#include <Game/Scripts/Projectile.hpp>

const std::string DefaultWeapon::Name = "Default";

DefaultWeapon::DefaultWeapon()
{
    this->_tag = "Projectile";
    this->_attributes["FireRate"] = new Attribute(1.0);
    this->_attributes["Ammo"] = new Attribute(100.0);
    this->_attributes["Damage"] = new Attribute(35.0);
    this->_attributes["CriticalChance"] = new Attribute(5.0 / 100.0);
    this->_attributes["CriticalStrike"] = new Attribute(150.0 / 100.0);

    _shootSound = EventSound::getEventByEventType(eEventSound::PLAYER_SHOOT);
    _material = ResourceManager::getInstance()->getResource<Material>("weapon_default.mat");
}

void    DefaultWeapon::upgradeByLevel()
{
    switch (this->_level)
    {
    case 1:
        this->_attributes["Damage"]->addModifier(Modifier(100.0 / 100.0));
        break;
    case 2:
        this->_tag = "ProjectileKnockBack";
        break;
    case 3:
        this->_attributes["CriticalChance"]->addModifier(Modifier(7.0));
        break;
    default:
        break;
    }
}

void    DefaultWeapon::fire(Player* player, sTransformComponent* playerTransform, sRenderComponent* playerRender, glm::vec3 playerDirection)
{
    Entity*                 bullet;
    sScriptComponent*       bulletScripts;
    Projectile*             projectileScript;

    this->_attributes["Ammo"]->addBonus(Bonus(-1));

    if (this->_attributes["Ammo"]->getFinalValue() == 0)
        this->reload();

    bullet = EntityFactory::createEntity("PLAYER_BULLET");
    bullet->setTag(this->_tag);
    bulletScripts = bullet->getComponent<sScriptComponent>();
    projectileScript = bulletScripts->getScript<Projectile>("Projectile");

    projectileScript->_projectileTransform->setPos(playerTransform->getPos());

    projectileScript->_projectileTransform->translate(glm::vec3(0.0f, -((playerRender->getModel()->getMin().y * playerTransform->getScale().y) / 2.0f), 0.0f));

    if (Maths::randomFrom(0.0f, 1.0f) < (1.0f - this->_attributes["CriticalChance"]->getFinalValue()))
        projectileScript->_damage = this->_attributes["Damage"]->getFinalValue();
    else
        projectileScript->_damage = this->_attributes["Damage"]->getFinalValue() * (1 + this->_attributes["CriticalStrike"]->getFinalValue());

    projectileScript->followDirection({ playerDirection.x, 0.0f, playerDirection.z });

#if (ENABLE_SOUND)
    if (_shootSound->soundID != -1)
    {
        SoundManager::getInstance()->playSound(_shootSound->soundID);
    }
#endif
}

void    DefaultWeapon::reload()
{
    this->_attributes["Ammo"]->clearAllBonuses();
}

void    DefaultWeapon::clean()
{

}