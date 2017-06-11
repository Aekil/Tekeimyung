/**
* @Author   Guillaume Labey
*/

#include <Engine/Core/Components/DynamicComponentFactory.hpp>

sComponent* ComponentFactory<sDynamicComponent>::loadFromJson(const std::string& entityType, const JsonValue& json)
{
    sDynamicComponent*  component;

    component = new sDynamicComponent();

    return component;
}

JsonValue&    ComponentFactory<sDynamicComponent>::saveToJson(const std::string& entityType, const sComponent* savedComponent, JsonValue* toJson)
{
    JsonValue& json = toJson ? *toJson : _componentsJson[entityType];
    const sDynamicComponent* component = static_cast<const sDynamicComponent*>(savedComponent ? savedComponent : _components[entityType]);

    return (json);
}
