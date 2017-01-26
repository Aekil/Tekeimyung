/**
* @Author   Guillaume Labey
*/

#include <imgui.h>

#include <Engine/EntityFactory.hpp>
#include <Engine/LevelEntitiesDebugWindow.hpp>
#include <Engine/Utils/Helper.hpp>


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

        // Add component to entity
        if (ImGui::Button("Add component"))
        {
            ImGui::OpenPopup("components");
        }

        // Save entity template to json
        // And apply changes to all entities of the same type in the scene
        ImGui::SameLine();
        if (ImGui::Button("Apply changes to template"))
        {
            EntityFactory::saveEntityTemplate(entityName, entity);
            EntityFactory::saveEntityTemplateToJson(entityName);
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

uint32_t    LevelEntitiesDebugWindow::getSelectedEntityId()
{
    return (_selectedEntityId);
}
