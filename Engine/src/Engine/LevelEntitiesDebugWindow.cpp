/**
* @Author   Guillaume Labey
*/

#include <imgui.h>

#include <Engine/EntityFactory.hpp>
#include <Engine/LevelEntitiesDebugWindow.hpp>

uint32_t    LevelEntitiesDebugWindow::_selectedEntityId = 0;

LevelEntitiesDebugWindow::LevelEntitiesDebugWindow(EntityManager* em, const glm::vec2& pos, const glm::vec2& size):
                                    _em(em), DebugWindow("Level entities", pos, size) {}

LevelEntitiesDebugWindow::~LevelEntitiesDebugWindow() {}

void    LevelEntitiesDebugWindow::build(float elapsedTime)
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
    static ImGuiTextFilter filter;

    filter.Draw();
    ImGui::BeginChild("Entities list", ImVec2(150, 0), true);
    for (Entity* entity: _em->getEntities())
    {
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
        selectedEntity = _em->getEntities().front();
        _selectedEntityId = selectedEntity->id;
    }

    displayEntityDebug(selectedEntity);

    ImGui::End();
}

void    LevelEntitiesDebugWindow::displayEntityDebug(Entity* entity)
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

        // Delete the entity
        if (ImGui::Button("Delete"))
        {
            ImGui::PopStyleColor(3);
            ImGui::EndGroup();
            _em->destroyEntity(entity);
            return;
        }

        // Add component to entity
        if (ImGui::Button("Add component"))
        {
            ImGui::OpenPopup("components");
        }

        ImGui::SameLine();
        // Clone the template
        if (ImGui::Button("Create template"))
        {
            ImGui::OpenPopup("create template");
        }

        // Clone template: specify new type name
        if (ImGui::BeginPopup("create template"))
        {
            static char newTypeName[64] = "";

            ImGui::InputText("New type", newTypeName, 64);
            if (ImGui::Button("Create"))
            {
                createTemplate(entity, newTypeName);
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }

        // Save entity template to json
        // And apply changes to all entities of the same type in the scene
        ImGui::SameLine();
        if (ImGui::Button("Apply changes to template"))
        {
            EntityFactory::saveEntityTemplate(entityName, entity);
        }

        ImGui::PopStyleColor(3);


        // Edit entity tag
        ImGui::Text("\n");
        const std::string& entityTag = entity->getTag();
        std::vector<char> entityTagVec(entityTag.cbegin(), entityTag.cend());
        entityTagVec.push_back(0);
        entityTagVec.resize(64);

        if (ImGui::InputText("Tag", entityTagVec.data(), entityTagVec.size()))
        {
            entity->setTag(entityTagVec.data());
        }


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

uint32_t    LevelEntitiesDebugWindow::getSelectedEntityId()
{
    return (_selectedEntityId);
}

void    LevelEntitiesDebugWindow::createTemplate(Entity* entity, const std::string& newTypeName)
{
    if (EntityFactory::entityTypeExists(newTypeName))
    {
        LOG_ERROR("Can't create entity type %s, it already exists", newTypeName.c_str());
        return;
    }

    sNameComponent* cloneTypeName = entity->getComponent<sNameComponent>();
    ASSERT(cloneTypeName != nullptr, "sNameComponent should exist");

    EntityFactory::createEntityType(newTypeName);

    EntityFactory::reverseAnimations(entity);

    // Save entity components
    auto& entityTypeInfos = EntityFactory::getInfos(cloneTypeName->value);
    const auto& entityTypeComponents = entity->getComponents();
    for (auto component: entityTypeComponents)
    {
        // Add cloned component to component factory
        std::string componentName = IComponentFactory::getComponentNameWithHash(component->id);
        auto compFactory = IComponentFactory::getFactory(componentName);
        sComponent* newComponent = component->clone();

        if (componentName == "sNameComponent")
        {
            sNameComponent* name = static_cast<sNameComponent*>(newComponent);
            name->value = newTypeName;
        }

        compFactory->save(newTypeName, newComponent);

        // Add component to EntityFactory
        // sNameComponent and sTransformComponent are already created in EntityFactory::createEntityType
        if (componentName != "sNameComponent" && componentName != "sTransformComponent")
        {
            EntityFactory::addComponent(newTypeName, componentName);
        }
    }

    EntityFactory::setTag(newTypeName, entityTypeInfos.tag);

    // Save the cloned template to json
    EntityFactory::saveEntityTemplateToJson(newTypeName);
}
