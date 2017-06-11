/**
* @Author   Guillaume Labey
*/

#include <Engine/Core/Components/UiComponentFactory.hpp>

constexpr unsigned int sUiComponent::identifier;

sComponent* ComponentFactory<sUiComponent>::loadFromJson(const std::string& entityType, const JsonValue& json)
{
    sUiComponent*  component;

    component = new sUiComponent();

    component->offset = json.getVec2f("offset", { 0.0f, 0.0f });
    component->layer = json.getInt("layer", 0);
    component->horizontalAlignment = EnumManager<eHorizontalAlignment>::stringToEnum(json.getString("horizontal_alignment", "MIDDLE"));
    component->verticalAlignment = EnumManager<eVerticalAlignment>::stringToEnum(json.getString("vertical_alignment", "MIDDLE"));
    component->percentageSize = json.getBool("percentage_size", false);
    component->size = json.getVec2f("size", {0.1f, 0.1f});

    return component;
}

JsonValue&    ComponentFactory<sUiComponent>::saveToJson(const std::string& entityType, const sComponent* savedComponent, JsonValue* toJson)
{
    JsonValue& json = toJson ? *toJson : _componentsJson[entityType];
    const sUiComponent* component = static_cast<const sUiComponent*>(savedComponent ? savedComponent : _components[entityType]);


    json.setVec2f("offset", component->offset);
    json.setInt("layer", component->layer);
    json.setString("horizontal_alignment", EnumManager<eHorizontalAlignment>::enumToString(component->horizontalAlignment));
    json.setString("vertical_alignment", EnumManager<eVerticalAlignment>::enumToString(component->verticalAlignment));
    json.setBool("percentage_size", component->percentageSize);
    json.setVec2f("size", component->size);

    return (json);
}

bool    ComponentFactory<sUiComponent>::updateEditor(const std::string& entityType, sComponent** savedComponent, sComponent* entityComponent, Entity* entity)
{
    sUiComponent* component = static_cast<sUiComponent*>(entityComponent ? entityComponent : _components[entityType]);
    *savedComponent = component;
    bool changed = false;

    int layer = component->layer;
    if (ImGui::InputInt("Layer", &layer, 1, ImGuiInputTextFlags_AllowTabInput))
    {
        component->layer = layer;
        changed = true;
    }

    changed |= Helper::updateComboEnum<eHorizontalAlignment>("Horizontal alignment", component->horizontalAlignment);
    changed |= Helper::updateComboEnum<eVerticalAlignment>("Vertical alignment", component->verticalAlignment);
    changed |= ImGui::InputFloat("Horizontal offset", &component->offset.x, 1.0f, ImGuiInputTextFlags_AllowTabInput);
    changed |= ImGui::InputFloat("Vertical offset", &component->offset.y, 1.0f, ImGuiInputTextFlags_AllowTabInput);
    changed |= ImGui::Checkbox("Percentage size", &component->percentageSize);

    if (component->percentageSize)
    {
        if (ImGui::InputFloat("Horizontal size", &component->size.x, 0.05f, ImGuiInputTextFlags_AllowTabInput))
        {
            changed = true;
            // Set size.x between 0 and 1
            component->size.x = std::min(component->size.x, 1.0f);
            component->size.x = std::max(component->size.x, 0.0f);
        }

        if (ImGui::InputFloat("Vertical size", &component->size.y, 0.05f, ImGuiInputTextFlags_AllowTabInput))
        {
            changed = true;
            // Set size.y between 0 and 1
            component->size.y = std::min(component->size.y, 1.0f);
            component->size.y = std::max(component->size.y, 0.0f);
        }
    }

    if (changed)
    {
        component->needUpdate = true;
    }

    return (changed);
}
