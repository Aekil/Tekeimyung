#include <imgui.h>

#include "EntityFactory.hpp"
#include "Utils/JsonWriter.hpp"

#include "EntityDebugWindow.hpp"


EntityDebugWindow::EntityDebugWindow(const glm::vec2& pos, const glm::vec2& size):
                                    DebugWindow("Live edition", pos, size), _selectedEntity(0) {}

EntityDebugWindow::~EntityDebugWindow() {}

void    EntityDebugWindow::build()
{
    static bool saveEntity = false;
    static bool spawnEntity = false;
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
    ImGui::ListBox("Entities types", &_selectedEntity, list, (int)EntityFactory::getTypesString().capacity(), 4);

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

        saveEntity = ImGui::Button("Save changes");
        spawnEntity = ImGui::Button("Spawn entity");
        if (saveEntity)
            saveEntityToJson(list[_selectedEntity]);

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
