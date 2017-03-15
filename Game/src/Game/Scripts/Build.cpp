/*
** @Author : Simon AMBROISE
*/

#include <Engine/Components.hh>

#include <Game/Scripts/Build.hpp>

void Build::start()
{
    this->_buildableItems.push_back("TOWER_FIRE");
    this->_buildableItems.push_back("TRAP_NEEDLE");
    this->_buildableItems.push_back("TRAP_CUTTER");
    this->_buildableItems.push_back("TRAP_FIRE");
    this->_buildEnabled = false;
    this->_buildableRadius = 5.7f;

    this->_transform = this->getComponent<sTransformComponent>();
    this->_render = this->getComponent<sRenderComponent>();
}

void Build::update(float dt)
{
    if (this->keyboard[Keyboard::eKey::B] == Keyboard::eKeyState::KEY_PRESSED)
        this->_buildEnabled = !this->_buildEnabled;

    this->checkBuildableZone();
}

void Build::checkBuildableZone()
{
    const auto& tiles = this->GetEntitiesByTag("BlockBrown");
    for (auto &tile : tiles)
    {
        auto box = tile->getComponent<sBoxColliderComponent>();
        if (box != nullptr)
        {
            auto pos = tile->getComponent<sTransformComponent>()->getPos();
            auto render = tile->getComponent<sRenderComponent>();
            auto scriptComponent = tile->getComponent<sScriptComponent>();

            if (!scriptComponent)
                continue;

            Tile* tile = scriptComponent->getScript<Tile>("Tile");
            if (!tile)
                continue;

            render->color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
            tile->setBuildable(false);

            if (!_buildEnabled)
                continue;

            if (Collisions::sphereVSAABB(_transform->getPos(), this->_buildableRadius * SIZE_UNIT, box->pos + pos, glm::vec3(box->size.x * SIZE_UNIT, box->size.y * SIZE_UNIT, box->size.z * SIZE_UNIT)))
            {
                tile->setBuildable(true);
                render->color = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
            }
        }
    }
}