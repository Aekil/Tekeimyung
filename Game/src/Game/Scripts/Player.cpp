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
    //LOG_DEBUG("I'm dead now");
}

void Player::start()
{
    setHealth(200);
    setMaxHealth(200);
    this->buildableRadius = 5.7f;
    this->_transform = this->getComponent<sTransformComponent>();
    this->_render = this->getComponent<sRenderComponent>();
    this->_rigidBody = this->getComponent<sRigidBodyComponent>();
    _buildEnabled = false;
    _damage = 20;
    _speed = 80.0f;

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
    //this->handleShoot();

    // Player is on top layer
    if (_waveManager && !_waveManager->isWaiting())
    {
        this->blockPlayerOnTopLayer(dt);
    }
}

void Player::updateDirection()
{
    Cursor& cursor = GameWindow::getInstance()->getMouse().getCursor();
    Camera* camera = Renderer::getInstance()->getCurrentCamera();

    if (!camera)
        return;

    Ray ray = camera->screenPosToRay((float)cursor.getX(), (float)cursor.getY());
    float hitDistance;

    if (Physics::raycastPlane(ray, {0.0f, 1.0f, 0.0f}, {0.0f, _transform->getPos().y, 0.0f}, hitDistance))
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

void Player::handleShoot()
{
    if (mouse.getStateMap()[Mouse::eButton::MOUSE_BUTTON_1] == Mouse::eButtonState::CLICK_PRESSED)
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
        projectileScript->followDirection({_direction.x, 0.0f, _direction.z});
    }
}

void Player::blockPlayerOnTopLayer(float dt)
{
    glm::vec3 playerPos = _transform->getPos();
    playerPos.x += (_rigidBody->velocity.x * dt);
    playerPos.z += (_rigidBody->velocity.z * dt);

    float modelSize = _render->getModel()->getSize().x / 2.0f;

    blockPlayer(playerPos);
    blockPlayer(playerPos + glm::vec3(modelSize, 0.0f, 0.0f));
    blockPlayer(playerPos + glm::vec3(0.0f, 0.0f, modelSize));
}

void Player::blockPlayer(const glm::vec3& playerPos)
{
    glm::ivec2 mapPos;
    mapPos.x = static_cast<int>(std::ceil(playerPos.x) / 25.0f);
    mapPos.y = static_cast<int>(playerPos.z / 25.0f);

    if (mapPos.x >= 0 &&
        mapPos.x < _gameManager->mapSize &&
        mapPos.y >= 0 &&
        mapPos.y < _gameManager->mapSize)
    {
        if (_gameManager->secondLayerPattern[mapPos.x][mapPos.y] != 1)
        {
            _rigidBody->velocity = glm::vec3(0.0f);
        }
    }
}

void Player::onHoverEnter()
{
    auto renderComponent = this->getComponent<sRenderComponent>();

    renderComponent->color = glm::vec4(1.0f, 1.0f, 0.0f, 0.5f);
}

void Player::onHoverExit()
{
    auto renderComponent = this->getComponent<sRenderComponent>();

    renderComponent->color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
}

void Player::onCollisionEnter(Entity* entity)
{
    if (entity->getComponent<sNameComponent>()->value == "TRAP_NEEDLE")
    {
        this->takeDamage(25);
    }
}

void Player::onCollisionExit(Entity* entity)
{
}
