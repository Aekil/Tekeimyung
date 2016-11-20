#include <imgui.h>

#include <Engine/Utils/Helper.hpp>

#include <Game/EntityFactory.hpp>
#include <Game/Utils/JsonWriter.hpp>
#include <Game/CollisionMap.hpp>

#include <Game/EntityDebugWindow.hpp>


EntityDebugWindow::EntityDebugWindow(Map* map, const glm::vec2& pos, const glm::vec2& size):
                                    _map(map), DebugWindow("Live edition", pos, size), _selectedEntity(0) {}

EntityDebugWindow::~EntityDebugWindow() {}

void    EntityDebugWindow::build()
{
    static bool saveEntityButton = false;
    static bool spawnEntityButton = false;
    std::vector<const char*>& typesString = const_cast<std::vector<const char*>&>(EntityFactory::getTypesString());
    const char** list = typesString.data();

    if (!ImGui::Begin(_title.c_str(), &_displayed, ImGuiWindowFlags_NoResize))
    {
        ImGui::End();
        return;
    }
    // Set Window params
    ImGui::SetWindowSize(ImVec2(_size.x, _size.y), ImGuiSetCond_Always);
    ImGui::SetWindowPos(ImVec2(_pos.x, _pos.y), ImGuiSetCond_Always);

    // Draw components
    ImGui::ListBox("Entities types", &_selectedEntity, list, (int)EntityFactory::getTypesString().size(), 4);

    const char* entityName = list[_selectedEntity];


    if (ImGui::CollapsingHeader(entityName, ImGuiTreeNodeFlags_DefaultOpen))
    {
        // Iterate over all components names
        auto&& components = EntityFactory::getComponents(entityName);
        for (auto &&componentName: components)
        {
            IComponentFactory* compFactory = IComponentFactory::getFactory(componentName);
            sComponent* component = nullptr;

            // The component data has changed
            if (compFactory->updateEditor(entityName, &component))
            {
                ASSERT(component != nullptr, "component should be set in updateEditor");
                EntityFactory::updateEntityComponent(entityName, compFactory, component);
            }
        }

        saveEntityButton = ImGui::Button("Save changes");
        spawnEntityButton = ImGui::Button("Spawn entity");
        if (saveEntityButton)
            saveEntityToJson(list[_selectedEntity]);
        else if (spawnEntityButton)
            spawnEntity(list[_selectedEntity]);

    }

    ImGui::End();
}

void    EntityDebugWindow::saveEntityToJson(const std::string& typeName)
{
    JsonWriter jsonWriter;
    JsonValue json;
    JsonValue components;
    auto&& entitycomponents = EntityFactory::getComponents(typeName);

    json.setString("name", typeName);
    for (auto &&component: entitycomponents)
    {
        JsonValue& componentJson = IComponentFactory::getFactory(component)->saveToJson(typeName, component);
        components.setValue(component, componentJson);
    }
    json.setValue("components", components);

    jsonWriter.write(EntityFactory::getFile(typeName), json);

    LOG_INFO("Entity %s saved", typeName.c_str());
}

glm::vec3   EntityDebugWindow::getRandomPos()
{
    CollisionMap* collisionMap = _map->getCollisionMap();
    eColType colType = eColType::CAN_NOT_WALK;
    glm::vec3 pos;

    while (colType == eColType::CAN_NOT_WALK)
    {
        pos.x = (float)Helper::randInt(0, _map->getWidth() - 1);
        pos.y = (float)Helper::randInt(0, _map->getHeight() - 1);
        pos.z = (float)Helper::randInt(1, _map->getLayersNb() - 1);

        colType = (*collisionMap)[(uint16_t)(pos.z - 1)][(uint32_t)std::floor(pos.y)][(uint32_t)std::floor(pos.x)];
    }

    return (pos);
}

void    EntityDebugWindow::spawnEntity(const std::string& typeName)
{
    Entity* entity = EntityFactory::createEntity(typeName);
    sTypeComponent* type = entity->getComponent<sTypeComponent>();
    sPositionComponent* pos = entity->getComponent<sPositionComponent>();

    if (!type || !pos)
    {
        LOG_ERROR("The entity type %s should have a sTypeComponent and sPositionComponent to appear on the map", typeName.c_str());
        return;
    }

    glm::vec3 randPos = getRandomPos();
    pos->value.x = randPos.x;
    pos->value.y = randPos.y;
    pos->z = randPos.z;
    LOG_INFO("Spawn entity %s at (%f,%f) on layer %f", typeName.c_str(), randPos.x, randPos.y, randPos.z);

    // Add entity to map entities list
    if (type->type == eEntityType::CHARACTER)
    {
        (*_map)[(uint16_t)randPos.z].addEntity(entity->id);
    }
    // Add entity to map
    // OBJECTS are particles emitters that should not be stored in the map
    else if (type->type != eEntityType::OBJECT)
    {
        (*_map)[(uint16_t)randPos.z][(uint32_t)randPos.y][(uint32_t)randPos.x] = entity->id;
    }
}
