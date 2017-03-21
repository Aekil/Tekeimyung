/*
** Author : Simon AMBROISE
*/

#include <cmath>

#include <Engine/Components.hh>
#include <Engine/EntityFactory.hpp>
#include <Engine/Graphics/Camera.hpp>
#include <Engine/Graphics/Renderer.hpp>
#include <Engine/Physics/Physics.hpp>

#include <Game/Scripts/GameManager.hpp>
#include <Game/Scripts/Projectile.hpp>
#include <Game/Scripts/Player.hpp>
#include <Game/Scripts/WaveManager.hpp>

void Player::death()
{
    this->Destroy();
}

void Player::start()
{
    setHealth(200);
    setMaxHealth(200);
    this->_transform = this->getComponent<sTransformComponent>();
    this->_render = this->getComponent<sRenderComponent>();
    this->_rigidBody = this->getComponent<sRigidBodyComponent>();
    _buildEnabled = false;
    _damage = 20;
    _speed = 80.0f;
    _shootSound = EventSound::getEventByEventType(eEventSound::PLAYER_SHOOT);

    // Get GameManager
    {
        auto em = EntityFactory::getBindedEntityManager();
        Entity* gameManager = em->getEntityByTag("GameManager");
        if (!gameManager)
        {
            LOG_WARN("Can't find entity with GameManager tag");
            return;
        }

        auto scriptComponent = gameManager->getComponent<sScriptComponent>();

        if (!scriptComponent)
        {
            LOG_WARN("Can't find scriptComponent on GameManager entity");
            return;
        }

        _gameManager = scriptComponent->getScript<GameManager>("GameManager");
        _waveManager = scriptComponent->getScript<WaveManager>("WaveManager");
    }

    //LOG_DEBUG("BORN");
}

void Player::update(float dt)
{
    this->updateDirection();
    this->movement(dt);
    this->handleShoot(dt);
}

void Player::updateDirection()
{
    Cursor& cursor = GameWindow::getInstance()->getMouse().getCursor();
    Camera* camera = Renderer::getInstance()->getCurrentCamera();

    if (!camera)
        return;

    Ray ray = camera->screenPosToRay((float)cursor.getX(), (float)cursor.getY());
    float hitDistance;

    if (Physics::raycastPlane(ray, { 0.0f, 1.0f, 0.0f }, { 0.0f, _transform->getPos().y, 0.0f }, hitDistance))
    {
        glm::vec3 target = ray.getPoint(hitDistance);
        _direction = glm::normalize(target - _transform->getPos());
    }
}

void Player::movement(float elapsedTime)
{
    // update player rotation
    {
        float rotation = glm::degrees(std::atan2(_direction.x, _direction.z));
        _transform->setRotation(glm::vec3(_transform->getRotation().x, rotation, _transform->getRotation().z));
    }

    if (!_rigidBody)
    {
        return;
    }

    _rigidBody->velocity = glm::vec3(0.0f);

    // update player position
    {
        if (KB_P(Keyboard::eKey::S))
        {
            _rigidBody->velocity += glm::vec3(_speed, 0.0f, _speed);
        }
        if (KB_P(Keyboard::eKey::Z))
        {
            _rigidBody->velocity += glm::vec3(-_speed, 0.0f, -_speed);
        }
        if (KB_P(Keyboard::eKey::Q))
        {
            _rigidBody->velocity += glm::vec3(-_speed, 0.0f, _speed);
        }
        if (KB_P(Keyboard::eKey::D))
        {
            _rigidBody->velocity += glm::vec3(_speed, 0.0f, -_speed);
        }
    }
}

void Player::handleShoot(float dt)
{
    this->_elapsedTime += dt;

    if (this->_elapsedTime > this->_fireRate)
    {
        this->_elapsedTime = 0;
        
        if (mouse.isPressed(Mouse::eButton::MOUSE_BUTTON_1))
        {
            Entity*                 bullet;
            sScriptComponent*       bulletScripts;
            Projectile*             projectileScript;

            bullet = Instantiate("PLAYER_BULLET");
            bulletScripts = bullet->getComponent<sScriptComponent>();
            projectileScript = bulletScripts->getScript<Projectile>("Projectile");

            projectileScript->_projectileTransform->setPos(_transform->getPos());

            projectileScript->_projectileTransform->translate(glm::vec3(0.0f, -((_render->getModel()->getMin().y * _transform->getScale().y) / 2.0f), 0.0f));

            projectileScript->_damage = _damage;
            projectileScript->followDirection({ _direction.x, 0.0f, _direction.z });

            #if (ENABLE_SOUND)
            if (_shootSound->soundID != -1 && !SoundManager::getInstance()->isSoundPlaying(_shootSound->soundID))
            {
                SoundManager::getInstance()->playSound(_shootSound->soundID);
            }
            #endif
        }
    }
}

void Player::onHoverEnter()
{
}

void Player::onHoverExit()
{
}

void Player::onCollisionEnter(Entity* entity)
{
}

void Player::onCollisionExit(Entity* entity)
{
}

void Player::addExperience(int xp)
{
    this->_experience += xp;

    if (this->_experience > this->_nextLevelUp)
        this->levelUp();
}

void Player::levelUp()
{
    this->_nextLevelUp += 250;

    this->_level++;

    LOG_DEBUG("%d", this->_level);
}