/*
** Author : Simon AMBROISE
*/

#include <cmath>

#include <Engine/Components.hh>
#include <Engine/EntityFactory.hpp>
#include <Engine/Graphics/Camera.hpp>
#include <Engine/Graphics/Renderer.hpp>
#include <Engine/Physics/Collisions.hpp>
#include <Engine/Physics/Physics.hpp>

#include <Game/Scripts/Tile.hpp>
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
    this->_transform = this->getComponent<sTransformComponent>();
    _buildEnabled = false;

    LOG_DEBUG("BORN");
}

void Player::Update(float dt)
{
    if (this->keyboard[Keyboard::eKey::B] ==  Keyboard::eKeyState::KEY_PRESSED)
    {
        _buildEnabled = !_buildEnabled;
    }

    this->CheckBuildableZone();

    this->Movement(dt);
}

void Player::CheckBuildableZone()
{
    auto em = EntityFactory::getBindedEntityManager();

    for (auto &entity : em->getEntities())
    {
        auto box = entity.second->getComponent<sBoxColliderComponent>();
        if (box != nullptr)
        {
            auto pos = entity.second->getComponent<sTransformComponent>()->pos;
            auto render = entity.second->getComponent<sRenderComponent>();
            auto scriptComponent = entity.second->getComponent<sScriptComponent>();

            Tile* tile = scriptComponent->getScript<Tile>("Tile");
            if (tile)
            {
                tile->SetBuildable(false);
            }

            render->color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

            if (Collisions::sphereVSAABB(_transform->pos, this->buildableRadius * SIZE_UNIT, box->pos + pos, glm::vec3(box->size.x * SIZE_UNIT, box->size.y * SIZE_UNIT, box->size.z * SIZE_UNIT)))
            {
                if (_buildEnabled)
                {
                    if (tile)
                    {
                        tile->SetBuildable(true);
                    }
                    render->color = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
                }
            }
        }
    }
}

void Player::Movement(float elapsedTime)
{
    Cursor& cursor = GameWindow::getInstance()->getMouse().getCursor();
    Camera* camera = Renderer::getInstance()->getCurrentCamera();

    if (!camera)
        return;


    Ray ray = camera->screenPosToRay((float)cursor.getX(), (float)cursor.getY());
    float hitDistance;

    if (Physics::raycastPlane(ray, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, hitDistance))
    {
        glm::vec3 target = ray.getPoint(hitDistance);
        glm::vec3 dir = glm::normalize(target - _transform->pos);
        float rotation = glm::degrees(std::atan2(dir.x, dir.z));
        _transform->rotation.y = rotation;
        _transform->needUpdate = true;
    }

    if (KB_P(Keyboard::eKey::K))
    {
        _transform->pos += glm::vec3(1.0f, 0.0f, 1.0f);
        _transform->needUpdate = true;
    }
    if (KB_P(Keyboard::eKey::I))
    {
        _transform->pos += glm::vec3(-1.0f, 0.0f, -1.0f);
        _transform->needUpdate = true;
    }
    if (KB_P(Keyboard::eKey::J))
    {
        _transform->pos += glm::vec3(-1.0f, 0.0f, 1.0f);
        _transform->needUpdate = true;
    }
    if (KB_P(Keyboard::eKey::L))
    {
        _transform->pos += glm::vec3(1.0f, 0.0f, -1.0f);
        _transform->needUpdate = true;
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
