/*
** @Author : Simon AMBROISE
*/

#include <Engine/EntityFactory.hpp>
#include <Engine/Physics/Collisions.hpp>
#include <Engine/Utils/Exception.hpp>
#include <Engine/Utils/Maths.hpp>

#include <Game/Character/Enemy.hpp>
#include <Game/Character/Player.hpp>
#include <Game/Character/NewBuild.hpp>
#include <Game/Manager/GameManager.hpp>
#include <Game/Manager/GoldManager.hpp>
#include <Game/Manager/TutoManagerMessage.hpp>

void Enemy::start()
{
    this->_render = getComponent<sRenderComponent>();
    this->_render->_animator.play("rotation_enemy", true);
    setHealth(getMaxHealth());
    this->_transform = getComponent<sTransformComponent>();
    Health::init(_render);
    _dyingSound = EventSound::getEventByEventType(eEventSound::ENEMY_DYING);
    _earningCoins = EventSound::getEventByEventType(eEventSound::EARN_COINS_FROM_ENEMY);

    auto& meshInstances = this->_render->getModelInstance()->getMeshsInstances();
    for (auto& meshInstance : meshInstances)
        this->_baseBlooms.push_back(meshInstance->getMaterial()->getBloom());

    this->_rigidBody = this->getComponent<sRigidBodyComponent>();

    if (this->_rigidBody == nullptr)
        EXCEPT(NullptrException, "Could not retrieve %s from Entity with archetype \"%s\"", "sRigidBodyComponent", "ENEMY");

    this->_rigidBody->ignoredTags.push_back("Enemy");
}

void Enemy::update(float dt)
{
    if (_path.size() > 0 &&
        _pathProgress < _path.size())
    {
        glm::vec3 targetPos = { _path[_pathProgress].x, this->_transform->getPos().y, _path[_pathProgress].z };
        const glm::vec3& entityPos = this->_transform->getPos();
        glm::vec3 direction = glm::normalize(targetPos - entityPos);

        direction *= this->_speed;

        this->_rigidBody->velocity = direction;

        if (glm::length(direction * dt) > glm::length(targetPos - entityPos))
        {
            _pathProgress++;
        }
    }
    else
        _transform->translate(glm::vec3(0.0f, 0.0f, -this->_speed * dt));

    if (this->_percentExplosion != nullptr)
    {
        int i = 0;
        auto& meshInstances = this->_render->getModelInstance()->getMeshsInstances();
        for (auto& meshInstance : meshInstances)
        {
            auto material = meshInstance->getMaterial();

            auto baseBloom = this->_baseBlooms[i];
            material->setBloom(glm::vec4{ baseBloom.x * this->_percentExplosion->getFinalValue() * 100, baseBloom.y * this->_percentExplosion->getFinalValue() * 100, baseBloom.z * this->_percentExplosion->getFinalValue() * 100, baseBloom.w });
            i++;
        }
    }

    Health::update(_transform);
}

void    Enemy::setPercentExplosion(Attribute* percentExplosion)
{
    this->_percentExplosion = percentExplosion;
}

Attribute*  Enemy::getPercentExplosion()
{
    return this->_percentExplosion;
}

void    Enemy::onCollisionEnter(Entity* entity)
{
    if (entity->getTag() == "ProjectileKnockBack")
    {
        EntityManager* em = EntityFactory::getBindedEntityManager();
        const auto& player = em->getEntityByTag("Player");

        glm::vec3 targetPos = { player->getComponent<sTransformComponent>()->getPos().x, this->_transform->getPos().y, player->getComponent<sTransformComponent>()->getPos().z };
        const glm::vec3& entityPos = this->_transform->getPos();
        glm::vec3 direction = glm::normalize(targetPos - entityPos);

        direction *= -15;
        _transform->translate(direction);
    }
    else if (entity->getTag() != "Enemy" &&
        _path.size() > 0 &&
        _pathProgress < _path.size())
    {
        int8_t i = _pathProgress > 0 ? _pathProgress - 1 : 0;
        sTransformComponent* transform = entity->getComponent<sTransformComponent>();
        for (i; i < _path.size(); ++i)
        {
            if (transform->getPos().x == _path[i].x &&
                transform->getPos().z == _path[i].z)
            {
                destroyWall(entity, _path[i]);
                break;
            }
        }
    }
}

void Enemy::death()
{
    EntityManager* em = EntityFactory::getBindedEntityManager();

    if (this->_percentExplosion != nullptr)
    {
        if (Maths::randomFrom(0.0f, 1.0f) > (1.0f - this->_percentExplosion->getFinalValue()))
        {
            auto sphereExplosion = this->Instantiate("SPHERE_LASER_EXPLOSION", this->getComponent<sTransformComponent>()->getPos());

            sRenderComponent* renderSphereExplosion = sphereExplosion->getComponent<sRenderComponent>();
            renderSphereExplosion->_animator.play("growing_up", false);
            auto enemies = em->getEntitiesByTag("Enemy");

            for (auto& enemy : enemies)
            {
                if (enemy->handle != this->entity->handle && Collisions::isCollidingSphereAndEntity(this->_transform->getPos(), 7.7f, enemy))
                {
                    auto enemyScriptComponent = enemy->getComponent<sScriptComponent>();

                    if (enemyScriptComponent == nullptr)
                        EXCEPT(NullptrException, "Could not find %s on Entity %s", "sScriptComponent", "Enemy");

                    auto enemyScript = enemyScriptComponent->getScript<Enemy>("Enemy");

                    if (enemyScript == nullptr)
                        EXCEPT(NullptrException, "Could not find script %s on Entity %s", "Enemy", "Enemy");

                    enemyScript->takeDamage(50.0f);
                }
            }
        }
    }

    this->remove();

    // Add golds for enemy dying
    const auto&         gameManager = em->getEntityByTag(GAME_MANAGER_TAG);
    sScriptComponent*   scriptComp = gameManager->getComponent<sScriptComponent>();
    GoldManager*        goldManager = scriptComp->getScript<GoldManager>(GOLD_MANAGER_TAG);

    goldManager->addGolds(this->_goldEarned);

    auto player = em->getEntityByTag("Player");

    player->getComponent<sScriptComponent>()->getScript<Player>("Player")->addExperience(this->_experienceEarned);

#if (ENABLE_SOUND)
    if (_dyingSound->soundID != -1)
    {
        SoundManager::getInstance()->playSound(_dyingSound->soundID);
    }
    if (_earningCoins->soundID != -1)
    {
        SoundManager::getInstance()->playSound(_earningCoins->soundID);
    }
#endif
}

// Called to remove the enemy when "dying", without gaining golds or exp ?
void Enemy::remove()
{
    Entity* explosion = Instantiate("ENEMY_EXPLOSION");
    sTransformComponent* explosionTransform = explosion->getComponent<sTransformComponent>();
    sTransformComponent* entityTransform = entity->getComponent<sTransformComponent>();
    explosionTransform->setPos(entityTransform->getPos());

    this->Destroy();

    TutoManagerMessage::getInstance()->sendMessage(eTutoState::ENEMY_DEAD);
}

bool Enemy::takeDamage(double damage)
{
    this->_render->_animator.play("hit", false);
    return (Health::takeDamage(damage));
}

void Enemy::setPath(const std::vector<glm::vec3>& path)
{
    _path = path;
    _pathProgress = 0;
}

void Enemy::stun()
{
    this->_rigidBody->velocity = glm::vec3{ 0,0,0 };
}

void Enemy::destroyWall(Entity* entity, const glm::vec3& pos)
{
    EntityManager* em = EntityFactory::getBindedEntityManager();
    Entity* gameManager = em->getEntityByTag("GameManager");
    if (!gameManager)
    {
        LOG_WARN("Could not find entity with tag \"%s\"", "GameManager");
        return;
    }
    Entity* player = em->getEntityByTag("Player");
    if (!player)
    {
        LOG_WARN("Could not find entity with tag \"%s\"", "Player");
        return;
    }

    GameManager* gameManagerScript = this->getEntityScript<GameManager>(gameManager, "GameManager");
    if (!gameManagerScript)
    {
        LOG_WARN("Could not find script GameManager on entity GameManager");
        return;
    }

    NewBuild* newBuildScript = this->getEntityScript<NewBuild>(player, "NewBuild");
    if (!newBuildScript)
    {
        LOG_WARN("Could not find script NewBuild on entity NewBuild");
        return;
    }

    Destroy(entity);


    glm::ivec2 tilePos;
    tilePos.x = static_cast<int>(std::ceil(pos.x) / 25.0f);
    tilePos.y = static_cast<int>(pos.z / 25.0f);

    gameManagerScript->map[tilePos.x][tilePos.y] = 1;

    auto& towersLayer = gameManagerScript->map.getTowersLayer();
    auto& mapEntities = gameManagerScript->map.getEntities();
    if (towersLayer[tilePos.x][tilePos.y] != nullptr)
    {
        auto alreadyBuiltTile = std::find(newBuildScript->_alreadyBuiltTile.begin(),
                                            newBuildScript->_alreadyBuiltTile.end(),
                                            mapEntities[tilePos.x][tilePos.y]->handle);
        if (alreadyBuiltTile != newBuildScript->_alreadyBuiltTile.end())
        {
            newBuildScript->_alreadyBuiltTile.erase(alreadyBuiltTile);
        }
        Destroy(towersLayer[tilePos.x][tilePos.y]);
        towersLayer[tilePos.x][tilePos.y] = nullptr;
    }
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

    if (ImGui::InputInt("Experience earned", &this->_experienceEarned, 10, ImGuiInputTextFlags_AllowTabInput))
    {
        if (this->_experienceEarned < 0)
            this->_experienceEarned = 0;
        changed = true;
    }

    if (ImGui::InputInt("Gold earned", &this->_goldEarned, 10, ImGuiInputTextFlags_AllowTabInput))
    {
        if (this->_goldEarned < 0)
            this->_goldEarned = 0;
        changed = true;
    }

    return (changed);
}

JsonValue Enemy::saveToJson()
{
    JsonValue json;

    json.setFloat("speed", _speed);
    json.setUInt("health", getMaxHealth());
    json.setUInt("experience_earned", this->_experienceEarned);
    json.setUInt("gold_earned", this->_goldEarned);

    return (json);
}

void    Enemy::loadFromJson(const JsonValue& json)
{
    _speed = json.getFloat("speed", 50.0f);
    setMaxHealth(json.getUInt("health", 150));
    this->_experienceEarned = json.getUInt("experience_earned", 20);
    this->_goldEarned = json.getUInt("gold_earned", 20);
}
