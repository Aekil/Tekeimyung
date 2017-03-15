/*
** Author : Simon AMBROISE
*/

#include <cmath>

#include <Engine/Components.hh>
#include <Engine/EntityFactory.hpp>
#include <Engine/Graphics/Camera.hpp>
#include <Engine/Graphics/Renderer.hpp>
#include <Engine/Physics/Physics.hpp>

#include <Game/Scripts/Projectile.hpp>
#include <Game/Scripts/Player.hpp>

void Player::death()
{
    this->Destroy();

    LOG_DEBUG("I'm dead now");
}

void Player::start()
{
    this->health = 200;
    this->maxHealth = 200;
    this->_transform = this->getComponent<sTransformComponent>();
    this->_render = this->getComponent<sRenderComponent>();
    _damage = 20;

    LOG_DEBUG("BORN");
}

void Player::update(float dt)
{
    this->updateDirection();
    this->movement(dt);
    this->handleShoot();
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

    // update player position
    {
        if (KB_P(Keyboard::eKey::S))
        {
            _transform->translate(glm::vec3(1.0f, 0.0f, 1.0f));
        }
        if (KB_P(Keyboard::eKey::Z))
        {
            _transform->translate(glm::vec3(-1.0f, 0.0f, -1.0f));
        }
        if (KB_P(Keyboard::eKey::Q))
        {
            _transform->translate(glm::vec3(-1.0f, 0.0f, 1.0f));
        }
        if (KB_P(Keyboard::eKey::D))
        {
            _transform->translate(glm::vec3(1.0f, 0.0f, -1.0f));
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
    LOG_DEBUG("onCollisionEnter Player, anyang.");

    if (entity->getComponent<sNameComponent>()->value == "TRAP_NEEDLE")
    {
        this->takeDamage(25);
    }
}

void Player::onCollisionExit(Entity* entity)
{
    LOG_DEBUG("onCollisionExit Player, bisous.");
}
