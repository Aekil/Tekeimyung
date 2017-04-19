/*
** Author : Simon AMBROISE
*/

#include <cmath>

#include <Engine/Components.hh>
#include <Engine/EntityFactory.hpp>
#include <Engine/Graphics/Camera.hpp>
#include <Engine/Graphics/Renderer.hpp>
#include <Engine/Physics/Physics.hpp>
#include <Engine/Utils/Exception.hpp>

#include <Game/Scripts/GameManager.hpp>
#include <Game/Scripts/Projectile.hpp>
#include <Game/Scripts/Player.hpp>
#include <Game/Scripts/WaveManager.hpp>

#include <Game/Weapons/DefaultWeapon.hpp>
#include <Game/Weapons/TeslaWeapon.hpp>
#include <Game/Weapons/LaserWeapon.hpp>

void Player::death()
{
    this->Destroy();
}

void Player::start()
{
    this->_weapons.push_back(new DefaultWeapon());
    this->_weapons.push_back(new TeslaWeapon());
    this->_weapons.push_back(new LaserWeapon());

    this->_levelUpReward[2] = std::make_pair<std::string, double>("FireRate", -25.0 / 100.0);
    this->_levelUpReward[3] = std::make_pair<std::string, double>("FireRate", -25.0 / 100.0);

    this->_attributes["Speed"] = new Attribute(80.0f);

    setHealth(200);
    setMaxHealth(200);

    this->_transform = this->getComponent<sTransformComponent>();
    this->_render = this->getComponent<sRenderComponent>();
    this->_rigidBody = this->getComponent<sRigidBodyComponent>();
    _buildEnabled = false;

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

    updateWeaponMaterial();
}

void Player::update(float dt)
{
    this->updateDirection();
    this->movement(dt);
    this->handleShoot(dt);
    this->changeWeapon();
}

void Player::changeWeapon()
{
    if (this->keyboard.getStateMap()[Keyboard::eKey::X] == Keyboard::eKeyState::KEY_RELEASED)
    {
        this->_actualWeapon++;

        if (this->_actualWeapon >= this->_weapons.size())
            this->_actualWeapon = 0;

        updateWeaponMaterial();

        LOG_DEBUG("Actual weapon : %d", this->_actualWeapon);
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
            _rigidBody->velocity += glm::vec3(this->_attributes["Speed"]->getFinalValue(), 0.0f, this->_attributes["Speed"]->getFinalValue());
        }
        if (KB_P(Keyboard::eKey::Z))
        {
            _rigidBody->velocity += glm::vec3(-this->_attributes["Speed"]->getFinalValue(), 0.0f, -this->_attributes["Speed"]->getFinalValue());
        }
        if (KB_P(Keyboard::eKey::Q))
        {
            _rigidBody->velocity += glm::vec3(-this->_attributes["Speed"]->getFinalValue(), 0.0f, this->_attributes["Speed"]->getFinalValue());
        }
        if (KB_P(Keyboard::eKey::D))
        {
            _rigidBody->velocity += glm::vec3(this->_attributes["Speed"]->getFinalValue(), 0.0f, -this->_attributes["Speed"]->getFinalValue());
        }
    }

    #if defined(ENGINE_DEBUG)
    if (this->keyboard.getStateMap()[Keyboard::eKey::L] == Keyboard::eKeyState::KEY_RELEASED)
    {
        this->_weapons[this->_actualWeapon]->levelUp();
    }
    #endif
}

void Player::handleShoot(float dt)
{
    this->_elapsedTime += dt;

    if (this->_elapsedTime > this->_weapons[this->_actualWeapon]->getAttribute("FireRate"))
    {
        if (mouse.isPressed(Mouse::eButton::MOUSE_BUTTON_1))
        {
            this->_weapons[this->_actualWeapon]->fire(this, this->_transform, this->_render, this->_direction);
            this->_elapsedTime = 0;
        }
        else
        {
            if (this->_weapons[this->_actualWeapon]->getName() == LaserWeapon::Name)
            {
                LaserWeapon* obj = dynamic_cast<LaserWeapon*>(this->_weapons[this->_actualWeapon]);
                if (obj->_laser != nullptr)
                {
                    this->Destroy(obj->_laser);
                    obj->_laser = nullptr;
                }
            }
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

void Player::updateWeaponMaterial()
{
    Material* weaponMaterial = this->_weapons[this->_actualWeapon]->getMaterial();

    if (!weaponMaterial)
    {
        EXCEPT(InternalErrorException, "Missing material for weapon %d", this->_actualWeapon);
    }

    _render->getModelInstance()->setMaterial(weaponMaterial);
}

void Player::levelUp()
{
    this->_nextLevelUp += 250;

    this->_level++;

    std::pair<std::string, double> reward = this->_levelUpReward[this->_level];

    //this->_attributes[reward.first]->addModifier(Modifier(reward.second));
}
