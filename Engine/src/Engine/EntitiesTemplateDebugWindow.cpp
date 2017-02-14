/**
* @Author   Guillaume Labey
*/

#include <imgui.h>

#include <Engine/EntitiesTemplateDebugWindow.hpp>
#include <Engine/EntityFactory.hpp>
#include <Engine/Utils/Helper.hpp>


EntitiesTemplateDebugWindow::EntitiesTemplateDebugWindow(EntityManager* em, const glm::vec2& pos, const glm::vec2& size):
                                    _em(em), DebugWindow("Entities templates", pos, size) {}

EntitiesTemplateDebugWindow::~EntitiesTemplateDebugWindow() {}

void    EntitiesTemplateDebugWindow::build(float elapsedTime)
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
    ImGui::PushItemWidth(120);
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

    // Clone template: specify new type name
    if (ImGui::Button("Save all"))
    {
        for (const auto& type: EntityFactory::getTypesString())
        {
            EntityFactory::saveEntityTemplateToJson(type);
        }
    }

    // Templates list
    ImGui::BeginChild("Templates list", ImVec2(150, 0), true);
    for (const auto& type: EntityFactory::getTypesString())
    {
        if (ImGui::Selectable(type, _selectedEntityTemplate == type))
        {
            _selectedEntityTemplate = type;
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
        EntityFactory::addComponent(newTypeName, componentName);
    }

    EntityFactory::setTag(newTypeName, entityTypeInfos.tag);

    // Save the cloned template to json
    EntityFactory::saveEntityTemplateToJson(newTypeName);
}
