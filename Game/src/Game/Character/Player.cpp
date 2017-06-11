/*
** Author : Simon AMBROISE
*/

#include <cmath>

#include <Engine/Core/Components/Components.hh>
#include <Engine/EntityFactory.hpp>
#include <Engine/Graphics/Camera.hpp>
#include <Engine/Graphics/Renderer.hpp>
#include <Engine/Physics/Physics.hpp>
#include <Engine/Utils/Exception.hpp>

#include <Game/Character/Player.hpp>
#include <Game/Manager/GameManager.hpp>
#include <Game/Manager/TutoManagerMessage.hpp>
#include <Game/Manager/WaveManager.hpp>
#include <Game/Weapons/Projectile.hpp>

#include <Game/Weapons/DefaultWeapon.hpp>
#include <Game/Weapons/TeslaWeapon.hpp>
#include <Game/Weapons/LaserWeapon.hpp>

REGISTER_SCRIPT(Player);

void Player::death()
{
    this->Destroy();
}

void Player::start()
{
    this->_reloadingProgress.init("AMMO_BAR_EMPTY", "AMMO_BAR");
    this->_reloadingProgress.display(false);

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
    if (TutoManagerMessage::getInstance()->stateOnGoingOrDone(eTutoState::MOVE))
        this->movement(dt);
    if (TutoManagerMessage::getInstance()->stateOnGoingOrDone(eTutoState::SHOOT))
        this->handleShoot(dt);
    if (TutoManagerMessage::getInstance()->stateOnGoingOrDone(eTutoState::CHANGE_WEAPON))
        this->changeWeapon();
}

void Player::changeWeapon()
{
    auto &&scroll = mouse.getScroll();
    static double lastOffset = scroll.yOffset;

    if (lastOffset != scroll.yOffset &&
        !keyboard.isPressed(Keyboard::eKey::LEFT_CONTROL))
    {
        this->_weapons[this->_actualWeapon]->clean();

        if (this->mouse.getScroll().yOffset < 0)
            this->_actualWeapon++;
        else
            this->_actualWeapon--;

        if (this->_actualWeapon >= (int)this->_weapons.size())
            this->_actualWeapon = 0;
        else if (this->_actualWeapon < 0)
            this->_actualWeapon = static_cast<int>(this->_weapons.size() - 1);

        updateWeaponMaterial();
        TutoManagerMessage::getInstance()->sendMessage(eTutoState::CHANGE_WEAPON);
    }

    lastOffset = scroll.yOffset;
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
            TutoManagerMessage::getInstance()->sendMessage(eTutoState::MOVE);
        }
        if (KB_P(Keyboard::eKey::Z))
        {
            _rigidBody->velocity += glm::vec3(-this->_attributes["Speed"]->getFinalValue(), 0.0f, -this->_attributes["Speed"]->getFinalValue());
            TutoManagerMessage::getInstance()->sendMessage(eTutoState::MOVE);
        }
        if (KB_P(Keyboard::eKey::Q))
        {
            _rigidBody->velocity += glm::vec3(-this->_attributes["Speed"]->getFinalValue(), 0.0f, this->_attributes["Speed"]->getFinalValue());
            TutoManagerMessage::getInstance()->sendMessage(eTutoState::MOVE);
        }
        if (KB_P(Keyboard::eKey::D))
        {
            _rigidBody->velocity += glm::vec3(this->_attributes["Speed"]->getFinalValue(), 0.0f, -this->_attributes["Speed"]->getFinalValue());
            TutoManagerMessage::getInstance()->sendMessage(eTutoState::MOVE);
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

    if (keyboard.getStateMap()[Keyboard::eKey::R] == Keyboard::eKeyState::KEY_RELEASED)
        this->_weapons[this->_actualWeapon]->setReloading(true);

    if (this->_weapons[this->_actualWeapon]->isReloading())
    {
        this->_weapons[this->_actualWeapon]->clean();
        this->_reloadElapsedTime += dt;

        if (!this->_reloadingProgress.isDisplayed())
        {
            this->_reloadingProgress.display(true);
            this->_reloadingProgress.maxProgress = this->_weapons[this->_actualWeapon]->_attributes["ReloadingTime"]->getBaseValue();
            this->_reloadingProgress.currentProgress = this->_weapons[this->_actualWeapon]->getAttribute("ReloadingTime") - this->_reloadElapsedTime;
        }

        if (this->_reloadElapsedTime > this->_weapons[this->_actualWeapon]->getAttribute("ReloadingTime"))
        {
            this->_reloadElapsedTime = 0.0f;
            this->_weapons[this->_actualWeapon]->setReloading(false);
            this->_weapons[this->_actualWeapon]->reload();
            this->_reloadingProgress.display(false);
        }
        else
        {
            this->_reloadingProgress.currentProgress -= dt;
            this->_reloadingProgress.update();
            return;
        }
    }
    else if (!this->_weapons[this->_actualWeapon]->isReloading() && this->_reloadingProgress.isDisplayed())
        this->_reloadingProgress.display(false);

    if (this->_canShoot && this->_elapsedTime > this->_weapons[this->_actualWeapon]->getAttribute("FireRate"))
    {
        if (mouse.isPressed(Mouse::eButton::MOUSE_BUTTON_1))
        {
            this->_weapons[this->_actualWeapon]->fire(this, this->_transform, this->_render, this->_direction);
            this->_elapsedTime = 0;
            TutoManagerMessage::getInstance()->sendMessage(eTutoState::SHOOT);
        }
        else
            this->_weapons[this->_actualWeapon]->clean();
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

void Player::setCanShoot(bool canShoot)
{
    this->_canShoot = canShoot;
}

bool Player::updateEditor()
{
    bool changed = false;

    ImGui::Text("Weapons config");

    // Configs list
    {
        uint32_t i = 0;
        ImGui::BeginChild("Weapons config", ImVec2(0, 100), true);
        for (auto& weapon : this->_weapons)
        {
            ImGui::PushID(i);
            if (ImGui::Selectable(weapon->getName().c_str(), _selectedWeapon == i))
            {
                _selectedWeapon = i;
            }
            ImGui::PopID();
            i++;
        }
        ImGui::EndChild();
    }


    if (this->_weapons.size() == 0)
        return (false);

    // Configs editor
    {
        auto& weapon = this->_weapons[_selectedWeapon];

        ImGui::Text(FMT_MSG("Damage (with modifier) : (%.3f)", weapon->getAttribute("Damage")).c_str());

        for (auto &attribute : weapon->_attributes)
        {
            auto baseValue = static_cast<float>(attribute.second->getBaseValue());

            if (ImGui::InputFloat(attribute.first.c_str(), &baseValue, 1.0f, ImGuiInputTextFlags_AllowTabInput))
            {
                if (baseValue < 0)
                    baseValue = 0;
                attribute.second->setBaseValue(baseValue);
                changed = true;
            }
        }
    }

    return (changed);
}

IWeapon*&    Player::getActualWeapon()
{
    return this->_weapons[this->_actualWeapon];
}
