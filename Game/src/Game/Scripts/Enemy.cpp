/*
** @Author : Simon AMBROISE
*/

#include <Engine/Components.hh>
#include <Engine/EntityFactory.hpp>

#include <Game/Scripts/GameManager.hpp>
#include <Game/Scripts/GoldManager.hpp>

#include <Game/Scripts/Enemy.hpp>

void Enemy::start()
{
    this->_render = getComponent<sRenderComponent>();
    this->_render->_animator.play("rotation_enemy", true);
    setHealth(getMaxHealth());
    this->_transform = getComponent<sTransformComponent>();
    Health::init(_render);
    _dyingSound = EventSound::getEventByEventType(eEventSound::ENEMY_DYING);
    _earningCoins = EventSound::getEventByEventType(eEventSound::EARN_COINS_FROM_ENEMY);
    _hitCastle = EventSound::getEventByEventType(eEventSound::ENEMY_HIT_CASTLE);
}

void Enemy::update(float dt)
{
    if (_path.size() > 0 &&
        _pathProgress < _path.size())
    {
        glm::vec3 targetPos = {_path[_pathProgress].x, this->_transform->getPos().y, _path[_pathProgress].z};
        const glm::vec3& entityPos = this->_transform->getPos();
        glm::vec3 direction = glm::normalize(targetPos - entityPos);

        direction *= this->_speed * dt;
        _transform->translate(direction);

        if (glm::length(direction) > glm::length(targetPos - entityPos))
        {
            _pathProgress++;
        }
    }
    else
    {
        _transform->translate(glm::vec3(0.0f, 0.0f, -this->_speed * dt));
    }

    Health::update(_transform);
}

void    Enemy::onCollisionEnter(Entity* entity)
{
}

void Enemy::death()
{
    EntityManager* em = EntityFactory::getBindedEntityManager();

    Entity* explosion = Instantiate("ENEMY_EXPLOSION");
    sTransformComponent* explosionTransform = explosion->getComponent<sTransformComponent>();
    sTransformComponent* entityTransform = entity->getComponent<sTransformComponent>();
    explosionTransform->setPos(entityTransform->getPos());

    this->Destroy();

    // Add golds for enemy dying
    const auto& gameManager = em->getEntityByTag(GAME_MANAGER_TAG);
    sScriptComponent* scriptComp = gameManager->getComponent<sScriptComponent>();
    GoldManager* goldManager = scriptComp->getScript<GoldManager>(GOLD_MANAGER_TAG);
    goldManager->addGolds(10); // arbitrary number which needs to be replaced depending on the enemy archetype

#if (ENABLE_SOUND)
    if (_dyingSound->soundID != -1 && !SoundManager::getInstance()->isSoundPlaying(_dyingSound->soundID))
    {
        SoundManager::getInstance()->playSound(_dyingSound->soundID);
    }
    if (_earningCoins->soundID != -1 && !SoundManager::getInstance()->isSoundPlaying(_earningCoins->soundID))
    {
        SoundManager::getInstance()->playSound(_earningCoins->soundID);
    }
#endif
}

// Called to remove the enemy when "dying", without gaining golds or exp ?
void Enemy::remove()
{
    this->Destroy();
#if (ENABLE_SOUND) // can be put in Castle.cpp script (after calling this function)
    if (_hitCastle->soundID != -1 && !SoundManager::getInstance()->isSoundPlaying(_hitCastle->soundID))
    {
        SoundManager::getInstance()->playSound(_hitCastle->soundID);
    }
#endif
}

bool Enemy::takeDamage(int damage)
{
    this->_render->_animator.play("hit", false);
    return (Health::takeDamage(damage));
}

void Enemy::setPath(const std::vector<glm::vec3>& path)
{
    _path = path;
    _pathProgress = 0;
}

bool Enemy::updateEditor()
{
    bool changed = false;

    changed |= ImGui::InputFloat("Speed", &_speed, 10.0f, ImGuiInputTextFlags_AllowTabInput);

    int health = getMaxHealth();
    if (ImGui::InputInt("Health", &health, 10, ImGuiInputTextFlags_AllowTabInput))
    {
        if (health < 0)
            health = 0;
        setMaxHealth(health);
        changed = true;
    }

    return (changed);
}

JsonValue Enemy::saveToJson()
{
    JsonValue json;

    json.setFloat("speed", _speed);
    json.setUInt("health", getMaxHealth());

    return (json);
}

void    Enemy::loadFromJson(const JsonValue& json)
{
    _speed = json.getFloat("speed", 50.0f);
    setMaxHealth(json.getUInt("health", 150));
}
