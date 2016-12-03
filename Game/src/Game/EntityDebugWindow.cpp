#include <imgui.h>

#include <Engine/Utils/Helper.hpp>

#include <Game/EntityFactory.hpp>
#include <Game/Utils/JsonWriter.hpp>
#include <Game/CollisionMap.hpp>

#include <Game/EntityDebugWindow.hpp>


uint32_t    EntityDebugWindow::_selectedEntityId = 0;

EntityDebugWindow::EntityDebugWindow(EntityManager* em, Map* map, const glm::vec2& pos, const glm::vec2& size):
                                    _em(em), _map(map), DebugWindow("Live edition", pos, size) {}

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

    // Create entity type
    static char typeName[64];
    ImGui::InputText("##default", typeName, 64);
    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Button, ImColor(0.27f, 0.51f, 0.70f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor(0.39f, 0.58f, 0.92f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor(0.49f, 0.68f, 0.92f, 1.0f));
        if (ImGui::Button("Create entity type") && std::string(typeName).size() > 0)
    {
        if (EntityFactory::entityTypeExists(typeName))
            LOG_ERROR("Can't create entity type %s, it already exists", typeName);
        else
            EntityFactory::createEntityType(typeName);
    }
    ImGui::PopStyleColor(3);

    // Entities list
    static ImGuiTextFilter filter;

    filter.Draw();
    ImGui::BeginChild("Entities list", ImVec2(150, 0), true);
    for (auto it: _em->getEntities())
    {
        Entity* entity = it.second;
        sNameComponent* name = entity->getComponent<sNameComponent>();

        ASSERT(name != nullptr, "The entity should have a name");
        if (filter.PassFilter(name->value.c_str()) == true)
        {
            ImGui::PushID(entity->id);
            if (ImGui::Selectable(name->value.c_str(), _selectedEntityId == entity->id))
            {
                _selectedEntityId = entity->id;
            }
            ImGui::PopID();
        }
    }
    ImGui::EndChild();

    if (_em->getEntities().size() == 0)
    {
        ImGui::End();
        return;
    }

    Entity* selectedEntity = _em->getEntity(_selectedEntityId);

    // The entity has been deleted or none is selected
    if (!selectedEntity)
    {
        selectedEntity = _em->getEntities().begin()->second;
        _selectedEntityId = selectedEntity->id;
    }

    displayEntityDebug(selectedEntity);
    ImGui::End();
}

void    EntityDebugWindow::displayEntityDebug(Entity* entity)
{
    ImGui::SameLine();
    ImGui::BeginGroup();
    sNameComponent* nameComp = entity->getComponent<sNameComponent>();
    ASSERT(nameComp != nullptr, "The entity should have a name");

    std::string entityName = nameComp->value;
    ImGui::PushItemWidth(200);
    if (ImGui::CollapsingHeader(entityName.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
    {
        // Add component button
        ImGui::PushStyleColor(ImGuiCol_Button, ImColor(0.27f, 0.51f, 0.70f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor(0.39f, 0.58f, 0.92f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor(0.49f, 0.68f, 0.92f, 1.0f));
        if (ImGui::Button("Add component"))
        {
            ImGui::OpenPopup("components");
        }
        ImGui::SameLine();
        if (ImGui::Button("Apply changes to template"))
        {
            saveEntityTemplate(entityName, entity);
            saveEntityTemplateToJson(entityName);
        }
        ImGui::PopStyleColor(3);

        // Display new component that can be added
        if (ImGui::BeginPopup("components"))
        {
            for (auto componentHash: IComponentFactory::getComponentsTypesHashs())
            {
                // Entity does not have this component, display it
                if (!entity->hasComponent(componentHash.first))
                {
                    // Component button pressed, add it to the entity
                    if (ImGui::Button(componentHash.second.c_str()))
                    {
                        // Get component factory to create a new one
                        IComponentFactory* newComponentFactory = IComponentFactory::getFactory(componentHash.second);
                        ASSERT(newComponentFactory != nullptr, "The component factory should exist");

                        // Send fake entity name and json to loadFromJson to have the component fields initialized with default values
                        sComponent* newComponent = newComponentFactory->loadFromJson("", {});
                        entity->addComponent(newComponent);
                    }
                }
            }
            ImGui::EndPopup();
        }
        // Display all components debug
        for (uint32_t i = 0; i < entity->getComponents().size(); ++i)
        {
            sComponent* component = entity->getComponents()[i];
            std::string componentName = IComponentFactory::getComponentNameWithHash(component->id);
            ASSERT(componentName.size() > 0, "The component name should exist");
            IComponentFactory* compFactory = IComponentFactory::getFactory(componentName);
            sComponent* savedComponent = nullptr;

            // Display component debug
            if (ImGui::CollapsingHeader(componentName.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
            {
                // Remove component button
                ImGui::PushID(componentName.c_str());
                if (componentName != "sTransformComponent" &&
                    componentName != "sNameComponent" &&
                    ImGui::Button("Remove component"))
                {
                    entity->removeComponent(component);
                    --i;
                }
                // Component debug content
                else if (compFactory->updateEditor(entityName, &savedComponent, component, entity))
                {
                    ASSERT(component != nullptr, "component should be set in updateEditor");
                }
                ImGui::PopID();
            }
        }
    }
    ImGui::EndGroup();
}

uint32_t    EntityDebugWindow::getSelectedEntityId()
{
    return (_selectedEntityId);
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
    {
        auto &&components = EntityFactory::getComponents(typeName);
        // Remove deleted components from factories
        for (auto it = components.begin(); it != components.end();)
        {
            auto component = *it;
            std::size_t componentHash = IComponentFactory::getComponentHashWithName(component);
            auto compFactory = IComponentFactory::getFactory(component);
            ASSERT(compFactory != nullptr, "The factory should exist");

            // The component has been removed, delete it from EntityFactory and ComponentFactory
            if (!entity->hasComponent(componentHash))
            {
                ++it;
                EntityFactory::removeComponent(typeName, component);
                compFactory->remove(typeName);
            }
            else
                ++it;
        }
    }

    {
        auto &&components = EntityFactory::getComponents(typeName);

        // Reverse animations
        sRenderComponent* render = entity->getComponent<sRenderComponent>();
        if (render && render->_selectedAnimation)
        {
            sTransformComponent* transform = entity->getComponent<sTransformComponent>();

            render->_selectedAnimation->reset();
            render->_selectedAnimation->update();
            transform->updateTransform();
        }

        // Save entity components
        for (auto component: entity->getComponents())
        {
            std::string componentName = IComponentFactory::getComponentNameWithHash(component->id);
            auto compFactory = IComponentFactory::getFactory(componentName);
            ASSERT(compFactory != nullptr, "The factory should exist");

            compFactory->save(typeName, component);

            // The component does not exist in entityFactory
            if (componentName != "sNameComponent" && std::find(components.begin(), components.end(), componentName) == components.end())
            {
                // Add component to EntityFactory
                EntityFactory::addComponent(typeName, componentName);
            }

            // Update other entities component
            EntityFactory::updateEntitiesComponents(entity, typeName, compFactory, component);
        }
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
