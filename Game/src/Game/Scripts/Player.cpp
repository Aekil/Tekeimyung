/*
** Author : Simon AMBROISE
*/

#include <Engine/Components.hh>

#include <Game/Scripts/Player.hpp>

void Player::Death()
{
    LOG_DEBUG("I'm dead now");
}

void Player::Start()
{
    this->m_health = 150;

    LOG_DEBUG("BORN");
}

void Player::Update(float dt)
{
    //LOG_DEBUG("%f, %f, %f", position->value.x, position->value.y, position->z);
    //if (this->m_health > 0)
    //    this->m_health -= 1;
    //else
    //    this->Destroy();

    this->Movement(dt);
}

void Player::Movement(float elapsedTime)
{
    sTransformComponent* transform = this->getComponent<sTransformComponent>();
    sDirectionComponent* direction = this->getComponent<sDirectionComponent>();
    direction->value *= -1;

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
        direction->value.x += glm::cos(angle);
        direction->value.y += glm::sin(angle);
        transform->pos += glm::vec3(direction->value.x, 0, direction->value.y);
        transform->needUpdate = true;
    }
    else if (KB_P(Keyboard::eKey::K))
    {
        float angle = glm::radians(-transform->rotation.y + 90.0f);
        direction->value.x -= glm::cos(angle);
        direction->value.y -= glm::sin(angle);
        transform->pos += glm::vec3(direction->value.x, 0, direction->value.y);
        transform->needUpdate = true;
    }
}

void Player::OnHoverEnter()
{
    auto renderComponent = this->getComponent<sRenderComponent>();

    renderComponent->color = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
    //LOG_DEBUG("coucou toi je suis sur toi");
}

void Player::OnHoverExit()
{
    auto renderComponent = this->getComponent<sRenderComponent>();

    renderComponent->color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
}