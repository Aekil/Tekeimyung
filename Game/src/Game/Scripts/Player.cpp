/*
** Author : Simon AMBROISE
*/

#include <Engine/Components.hh>
#include <Engine/Physics/Collisions.hpp>
#include <Engine/EntityFactory.hpp>

#include <Game/Scripts/Player.hpp>

void Player::Death()
{
    this->Destroy();

    LOG_DEBUG("I'm dead now");
}

void Player::Start()
{
    this->health = 200;
    this->buildableRadius = 5.7f;

    LOG_DEBUG("BORN");
}

void Player::Update(float dt)
{
    this->CheckBuildableZone();
    this->Movement(dt);
}

void Player::CheckBuildableZone()
{
    auto em = EntityFactory::getBindedEntityManager();
    auto transform = this->getComponent<sTransformComponent>()->pos;

    for (auto &entity : em->getEntities())
    {
        auto box = entity.second->getComponent<sBoxColliderComponent>();
        if (box != nullptr)
        {
            auto pos = entity.second->getComponent<sTransformComponent>()->pos;
            auto render = entity.second->getComponent<sRenderComponent>();
            auto scriptComponent = entity.second->getComponent<sScriptComponent>();

            render->color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

            if (scriptComponent != nullptr)
            {
                if (std::find(scriptComponent->scriptNames.cbegin(), scriptComponent->scriptNames.cend(), "Tile") != scriptComponent->scriptNames.cend())
                {
                    auto index = std::find(scriptComponent->scriptNames.cbegin(), scriptComponent->scriptNames.cend(), "Tile") - scriptComponent->scriptNames.begin();

                    Tile* tile = (Tile*)scriptComponent->scriptInstances[index];
                    tile->SetBuildable(false);
                }
            }

            if (Collisions::sphereVSAABB(transform, this->buildableRadius * SIZE_UNIT, box->pos + pos, glm::vec3(box->size.x * SIZE_UNIT, box->size.y * SIZE_UNIT, box->size.z * SIZE_UNIT)))
            {
                if (entity.second->getComponent<sScriptComponent>() != nullptr)
                {
                    auto scriptComponent = entity.second->getComponent<sScriptComponent>();

                    if (std::find(scriptComponent->scriptNames.cbegin(), scriptComponent->scriptNames.cend(), "Tile") != scriptComponent->scriptNames.cend())
                    {
                        auto index = std::find(scriptComponent->scriptNames.cbegin(), scriptComponent->scriptNames.cend(), "Tile") - scriptComponent->scriptNames.begin();

                        Tile* tile = (Tile*)scriptComponent->scriptInstances[index];
                        tile->SetBuildable(true);
                    }
                }
                render->color = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
            }
        }
    }
}

void Player::Movement(float elapsedTime)
{
    sTransformComponent* transform = this->getComponent<sTransformComponent>();
    _direction *= -1;

    if (KB_P(Keyboard::eKey::J))
    {
        transform->rotation.y += 180.0f * elapsedTime;
        transform->rotation.y = std::fmod(transform->rotation.y, 360.0f);
        transform->needUpdate = true;
    }
    else if (KB_P(Keyboard::eKey::L))
    {
        transform->rotation.y -= 180.0f * elapsedTime;
        transform->rotation.y = std::fmod(transform->rotation.y, 360.0f);
        transform->needUpdate = true;
    }
    if (KB_P(Keyboard::eKey::I))
    {
        float angle = glm::radians(-transform->rotation.y + 90.0f);
        _direction.x += glm::cos(angle);
        _direction.y += glm::sin(angle);
        transform->pos += glm::vec3(_direction.x, 0, _direction.y);
        transform->needUpdate = true;
    }
    else if (KB_P(Keyboard::eKey::K))
    {
        float angle = glm::radians(-transform->rotation.y + 90.0f);
        _direction.x -= glm::cos(angle);
        _direction.y -= glm::sin(angle);
        transform->pos += glm::vec3(_direction.x, 0, _direction.y);
        transform->needUpdate = true;
    }
}

void Player::OnHoverEnter()
{
    auto renderComponent = this->getComponent<sRenderComponent>();

    renderComponent->color = glm::vec4(1.0f, 1.0f, 0.0f, 0.5f);
}

void Player::OnHoverExit()
{
    auto renderComponent = this->getComponent<sRenderComponent>();

    renderComponent->color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
}

void Player::OnCollisionEnter(Entity* entity)
{
    if (entity->getComponent<sNameComponent>()->value == "TRAP_NEEDLE")
    {
        this->TakeDamage(25);
    }
}
