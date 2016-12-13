/**
* @Author   Guillaume Labey
*/

#include <algorithm>

#include <Engine/Window/GameWindow.hpp>
#include <Engine/Utils/Logger.hpp>
#include <Engine/Sound/SoundManager.hpp>
#include <Game/Utils/EventSound.hpp>

#include <Game/EntityFactory.hpp>
#include <Game/Components.hh>

#include <Game/Systems/BuildSystem.hpp>

BuildSystem::BuildSystem(EntityManager* em, Map* map): _map(map), _em(em)
{
    addDependency<sPlayerComponent>();
    addDependency<sPositionComponent>();

    _monitoringKey = MonitoringDebugWindow::getInstance()->registerSystem(BUILD_SYSTEM_NAME);
}

BuildSystem::~BuildSystem() {}

bool    BuildSystem::init()
{
    _tower = EntityFactory::createEntity(eArchetype::TOWER_FIRE);
    return (true);
}

void    BuildSystem::update(EntityManager &em, float elapsedTime)
{
    Timer timer;

    Entity* selectedEntity = Map::getSelectedEntity(&em, true);

    // Draw player range
    forEachEntity(em, [&](Entity* entity) {
        sPlayerComponent* player = entity->getComponent<sPlayerComponent>();
        sPositionComponent* pos = entity->getComponent<sPositionComponent>();

        // Clear previous range
        drawRange(em, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::ivec3(player->lastPos.x, player->lastPos.y, player->lastPos.z), player->range);
        player->lastPos = glm::ivec3(pos->value, pos->z);
        // Draw new previous range
        drawRange(em, player->rangeColor, glm::ivec3(player->lastPos.x, player->lastPos.y, player->lastPos.z), player->range);

        if (selectedEntity)
        {
            sPositionComponent* selectedEntityPos = selectedEntity->getComponent<sPositionComponent>();
            glm::ivec3 tilePos = glm::ivec3(selectedEntityPos->value.x, selectedEntityPos->value.y, selectedEntityPos->z);
            if (!(tilePos.x == pos->value.x && tilePos.y == pos->value.y)  &&
                isInRange(player->lastPos, tilePos, player->range))
            {
                displayBuildItem(tilePos);
                buildItem(tilePos);
            }
            else
                hideBuildingItem();
        }
        else
            hideBuildingItem();
    });


    MonitoringDebugWindow::getInstance()->updateSystem(_monitoringKey, timer.getElapsedTime(), (uint32_t)_entities.size());
}

void    BuildSystem::drawRange(EntityManager &em, const glm::vec4& color, const glm::ivec3& playerPos, uint32_t range)
{
    glm::ivec3 rangePos = playerPos - (int)range;
    for (uint32_t x = 0; x <= range * 2; ++x)
    {
        for (uint32_t y = 0; y <= range * 2; ++y)
        {
            glm::ivec3 pos = glm::ivec3(rangePos.x + x, rangePos.y + y, playerPos.z - 1);
            if (!_map->isValidPosition(pos))
                continue;

            uint32_t entityId = (*_map)[pos.z][pos.y][pos.x].get();
            Entity* entity = em.getEntity(entityId);

            if (entity)
            {
                sRenderComponent* render = entity->getComponent<sRenderComponent>();
                if (render)
                    render->color = color;
            }
        }
    }
}

bool    BuildSystem::isInRange(const glm::ivec3& playerPos, const glm::ivec3& objPos, uint32_t range) const
{
    glm::ivec3 rangePos = playerPos - (int)range;

    // Check there is no tile above the obj position
    glm::ivec3 abovePos = glm::ivec3(objPos.x, objPos.y, objPos.z + 1);
    if (_map->isValidPosition(abovePos) &&
        _em->getEntity((*_map)[abovePos.z][abovePos.y][abovePos.x].get()))
        return (false);

    // Check it is in range
    return (objPos.z == playerPos.z - 1 &&
        objPos.x >= rangePos.x &&
        objPos.x <= rangePos.x + (int)range * 2 &&
        objPos.y >= rangePos.y &&
        objPos.y <= rangePos.y + (int)range * 2);
}

void    BuildSystem::displayBuildItem(const glm::ivec3& tilePos)
{
    sTransformComponent* transform = _tower->getComponent<sTransformComponent>();
    sRenderComponent* render = _tower->getComponent<sRenderComponent>();

    // Display it
    render->_display = true;

    // Set item on the tile
    transform->pos = Map::mapToGraphPosition(glm::vec2(tilePos.x, tilePos.y), (float)tilePos.z + 1);
    transform->needUpdate = true;
}

void    BuildSystem::hideBuildingItem()
{
    sTransformComponent* transform = _tower->getComponent<sTransformComponent>();
    sRenderComponent* render = _tower->getComponent<sRenderComponent>();

    // Hide the item
    render->_display = true;

    // Put it under the map so that it does not affect physics
    transform->pos = glm::vec3(0.0f, 0.0f, -1000.0f);
    transform->needUpdate = true;
}

void    BuildSystem::buildItem(const glm::ivec3& tilePos)
{
    auto &&mouse = GameWindow::getInstance()->getMouse();

    // Build item if the mouse is pressed
    if (mouse.getStateMap()[Mouse::eButton::MOUSE_BUTTON_1] == Mouse::eButtonState::CLICK_PRESSED)
    {
        // Get item item
        sNameComponent* name = _tower->getComponent<sNameComponent>();
        ASSERT(name != nullptr, "The entity should have a name");

        // Clone the item with his name
        Entity* item = EntityFactory::cloneEntity(name->value);

        // Set item position to tile position
        sPositionComponent* itemPos = item->getComponent<sPositionComponent>();

        itemPos->value.x = (float)tilePos.x;
        itemPos->value.y = (float)tilePos.y;
        itemPos->z = (float)tilePos.z + 1.0f;

        // Update item transform
        sTransformComponent* itemTransform = item->getComponent<sTransformComponent>();
        itemTransform->pos = Map::mapToGraphPosition(itemPos->value, itemPos->z);


        // Check no entity is on the tile
        (*_map)[(uint16_t)itemPos->z][(uint32_t)itemPos->value.y][(uint32_t)itemPos->value.x] = item->id;

        // Play sound
        #if (ENABLE_SOUND)
            SoundManager::getInstance()->playSound(EventSound::getSoundIDFromEvent(eEventSound::BUILD));
        #endif
    }
}
