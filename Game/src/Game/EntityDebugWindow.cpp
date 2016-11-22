#include <sstream>
#include <imgui.h>

#include <Engine/Utils/Helper.hpp>

#include <Game/EntityFactory.hpp>
#include <Game/Utils/JsonWriter.hpp>
#include <Game/CollisionMap.hpp>

#include <Game/EntityDebugWindow.hpp>


EntityDebugWindow::EntityDebugWindow(EntityManager* em, Map* map, const glm::vec2& pos, const glm::vec2& size):
                                    _em(em), _map(map), DebugWindow("Live edition", pos, size), _selectedEntity(0) {}

EntityDebugWindow::~EntityDebugWindow() {}

void    EntityDebugWindow::build()
{
    if (!ImGui::Begin(_title.c_str(), &_displayed, ImGuiWindowFlags_NoResize))
    {
        ImGui::End();
        return;
    }
    // Set Window params
    ImGui::SetWindowSize(ImVec2(_size.x, _size.y), ImGuiSetCond_Always);
    ImGui::SetWindowPos(ImVec2(_pos.x, _pos.y), ImGuiSetCond_Always);

    // Entities list
    static Entity* selectedEntity = nullptr;
    // Store entity id to handle entity deletion
    static uint32_t selectedEntityId = 0;
    ImGui::BeginChild("Entities list", ImVec2(150, 0), true);
    for (auto it: _em->getEntities())
    {
        std::stringstream name;
        Entity* entity = it.second;
        sNameComponent* nameComp = entity->getComponent<sNameComponent>();

        ASSERT(nameComp != nullptr, "The entity should have a name");
        name << "[" << entity->id << "] " << nameComp->value;
        if (ImGui::Selectable(name.str().c_str(), selectedEntity && selectedEntity->id == entity->id))
        {
            selectedEntity = entity;
            selectedEntityId = entity->id;
        }
    }
    ImGui::EndChild();

    // No entity selected
    if (!selectedEntity)
    {
        ImGui::End();
        return;
    }
    // The entity has been deleted
    else if (_em->getEntity(selectedEntityId) == nullptr)
    {
        selectedEntity = _em->getEntities().begin()->second;
        selectedEntityId = selectedEntity->id;
    }

    // Entity edition
    ImGui::SameLine();
    ImGui::BeginGroup();
    sNameComponent* nameComp = selectedEntity->getComponent<sNameComponent>();
    ASSERT(nameComp != nullptr, "The entity should have a name");

    std::string entityName = nameComp->value;
    if (ImGui::CollapsingHeader(entityName.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
    {
        for (auto component: selectedEntity->getComponents())
        {
            std::string componentName = IComponentFactory::getComponentNameWithHash(component->getTypeInfo().hash_code());
            ASSERT(componentName.size() > 0, "The component name should exist");
            IComponentFactory* compFactory = IComponentFactory::getFactory(componentName);
            sComponent* savedComponent = nullptr;

            // The component data has changed
            if (compFactory->updateEditor(entityName, &savedComponent, component))
            {
                ASSERT(component != nullptr, "component should be set in updateEditor");
            }
        }

        if (ImGui::Button("Apply changes to template"))
        {
            saveEntityTemplate(entityName, selectedEntity);
            saveEntityTemplateToJson(entityName);
        }
/*        else if (spawnEntityButton)
            spawnEntity(list[_selectedEntity]);*/
    }
    ImGui::EndGroup();
    ImGui::End();
}

void    EntityDebugWindow::saveEntityTemplateToJson(const std::string& typeName)
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

void    EntityDebugWindow::saveEntityTemplate(const std::string& typeName, Entity* entity)
{
    for (auto component: entity->getComponents())
    {
        std::string componentName = IComponentFactory::getComponentNameWithHash(component->getTypeInfo().hash_code());
        auto compFactory = IComponentFactory::getFactory(componentName);
        compFactory->save(typeName, component);

        // Update other entities component
        EntityFactory::updateEntityComponent(typeName, compFactory, component);
    }
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
