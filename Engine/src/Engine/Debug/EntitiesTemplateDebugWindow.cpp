/**
* @Author   Guillaume Labey
*/

#include <imgui.h>

#include <Engine/Core/Components/IComponentFactory.hpp>
#include <Engine/Core/Components/NameComponent.hh>
#include <Engine/EntityFactory.hpp>
#include <Engine/Utils/Helper.hpp>

#include <Engine/Debug/EntitiesTemplateDebugWindow.hpp>

EntitiesTemplateDebugWindow::EntitiesTemplateDebugWindow() : DebugWindow("Archetypes") {}

EntitiesTemplateDebugWindow::EntitiesTemplateDebugWindow(const glm::vec2& pos, const glm::vec2& size) :
                                    DebugWindow("Archetypes", pos, size) {}

EntitiesTemplateDebugWindow::~EntitiesTemplateDebugWindow() {}

void    EntitiesTemplateDebugWindow::build(std::shared_ptr<GameState> gameState, float elapsedTime)
{
    static ImGuiTextFilter  filter;

    if (!ImGui::Begin(_title.c_str(), &_displayed))
    {
        ImGui::End();
        return;
    }

    if (ImGui::Button("New archetype")) ImGui::OpenPopup("create_archetype");
    ImGui::SameLine();

    if (ImGui::BeginPopup("create_archetype"))
    {
        static char newArchetypeName[64] = "";

        ImGui::InputText("Archetype name", newArchetypeName, 64);
        if (std::string(newArchetypeName).size() > 0)
        {
            ImGui::Separator();
            if (ImGui::Button("Confirm") == true)
            {
                if (EntityFactory::entityTypeExists(newArchetypeName))
                    LOG_ERROR("Could not create archetype %s, it already exists", newArchetypeName);
                else
                    EntityFactory::createEntityType(newArchetypeName);
                ImGui::CloseCurrentPopup();
            }
        }
        ImGui::EndPopup();
    }

    // Clone template: specify new type name
    if (ImGui::Button("Save all"))
    {
        for (const auto& type : EntityFactory::getTypesString())
        {
            EntityFactory::saveEntityTemplateToJson(type);
        }
    }
    ImGui::SameLine();

    // Templates list
    filter.Draw();
    ImGui::BeginChild("Archetypes list", ImVec2(300, 0), true);
    for (const auto& type: EntityFactory::getTypesString())
    {
        if (filter.PassFilter(type))
        {
            if (ImGui::Selectable(type, _selectedEntityTemplate == type))
            {
                _selectedEntityTemplate = type;
            }
        }
    }
    ImGui::EndChild();

    ImGui::SameLine();
    ImGui::BeginGroup();

    if (_selectedEntityTemplate.size() > 0)
    {
        // Spawn new entity
        if (ImGui::Button("Spawn entity"))
        {
            EntityFactory::createEntity(_selectedEntityTemplate);
        }

        // Clone the template
        if (ImGui::Button("Clone template"))
        {
            ImGui::OpenPopup("clone template");
        }

        // Clone template: specify new type name
        if (ImGui::BeginPopup("clone template"))
        {
            static char newTypeName[64] = "";

            ImGui::InputText("New type", newTypeName, 64);
            if (ImGui::Button("Create"))
            {
                cloneTemplate(_selectedEntityTemplate, newTypeName);
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }

        // Clone template: specify new type name
        if (ImGui::Button("Save"))
        {
            EntityFactory::saveEntityTemplateToJson(_selectedEntityTemplate);
        }
    }


    ImGui::EndGroup();
    ImGui::End();
}

void    EntitiesTemplateDebugWindow::cloneTemplate(const std::string& cloneTypeName, const std::string& newTypeName)
{
    if (EntityFactory::entityTypeExists(newTypeName))
    {
        LOG_ERROR("Can't create entity type %s, it already exists", newTypeName.c_str());
        return;
    }

    EntityFactory::createEntityType(newTypeName);

    // Save entity components
    auto& entityTypeInfos = EntityFactory::getInfos(cloneTypeName);
    const auto& entityTypeComponents = entityTypeInfos.components;
    for (auto componentName: entityTypeComponents)
    {
        auto compFactory = IComponentFactory::getFactory(componentName);
        ASSERT(compFactory != nullptr, "The factory should exist");

        // Add cloned component to component factory
        sComponent* newComponent = compFactory->clone(cloneTypeName);

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
