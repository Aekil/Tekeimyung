/**
* @Author   Guillaume Labey
*/

#include <Engine/Utils/LevelLoader.hpp>

#include <Engine/Core/Components/ButtonComponentFactory.hpp>

sComponent* ComponentFactory<sButtonComponent>::loadFromJson(const std::string& entityType, const JsonValue& json)
{
    sButtonComponent*  component;

    component = new sButtonComponent();

    component->action = EnumManager<sButtonComponent::eAction>::stringToEnum(json.getString("action", "NONE"));
    component->actionLevel = json.getString("action_level", "");
    component->removeStates = json.getUInt("remove_states", 0);

    return component;
}

JsonValue&    ComponentFactory<sButtonComponent>::saveToJson(const std::string& entityType, const sComponent* savedComponent, JsonValue* toJson)
{
    JsonValue& json = toJson ? *toJson : _componentsJson[entityType];
    const sButtonComponent* component = static_cast<const sButtonComponent*>(savedComponent ? savedComponent : _components[entityType]);

    json.setString("action", EnumManager<sButtonComponent::eAction>::enumToString(component->action));
    json.setString("action_level", component->actionLevel);
    json.setUInt("remove_states", component->removeStates);

    return (json);
}

bool    ComponentFactory<sButtonComponent>::updateEditor(const std::string& entityType, sComponent** savedComponent, sComponent* entityComponent, Entity* entity)
{
    sButtonComponent* component = static_cast<sButtonComponent*>(entityComponent ? entityComponent : _components[entityType]);
    *savedComponent = component;
    bool changed = false;

    changed |= Helper::updateComboEnum<sButtonComponent::eAction>("Action", component->action);

    int removeStates = component->removeStates;
    if (ImGui::InputInt("Remove states before action", &removeStates))
    {
        removeStates = std::max(removeStates, 0);
        component->removeStates = removeStates;
    }

    if (component->action == sButtonComponent::eAction::ADD_LEVEL ||
        component->action == sButtonComponent::eAction::REPLACE_CURRENT_LEVEL)
    {
        Helper::updateComboString("Level", LevelLoader::getInstance()->getLevels(), component->actionLevel);
    }

    return (changed);
}
