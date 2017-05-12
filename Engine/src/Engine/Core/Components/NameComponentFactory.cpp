#include <Engine/Core/Components/NameComponentFactory.hpp>

sComponent* ComponentFactory<sNameComponent>::loadFromJson(const std::string& entityType, const JsonValue& json)
{
    sNameComponent*   component;

    component = new sNameComponent();

    component->value = json.getString("name", "default");

    return (component);
}

JsonValue&    ComponentFactory<sNameComponent>::saveToJson(const std::string& entityType, const sComponent* savedComponent, JsonValue* toJson)
{
    JsonValue& json = toJson ? *toJson : _componentsJson[entityType];
    const sNameComponent* component = static_cast<const sNameComponent*>(savedComponent ? savedComponent : _components[entityType]);

    json.setString("name", component->value);

    return (json);
}
