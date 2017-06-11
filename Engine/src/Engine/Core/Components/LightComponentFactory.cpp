/**
* @Author   Guillaume Labey
*/

#include <glm/gtc/type_ptr.hpp>

#include <Engine/Core/Components/LightComponentFactory.hpp>

constexpr unsigned int sLightComponent::identifier;

sComponent* ComponentFactory<sLightComponent>::loadFromJson(const std::string& entityType, const JsonValue& json)
{
    sLightComponent*  component;

    component = new sLightComponent();

    component->light.setAmbient(json.getVec3f("ambient", {0.3f, 0.3f, 0.3f}));
    component->light.setDiffuse(json.getVec3f("diffuse", {1.0f, 1.0f, 1.0f}));
    component->light.setDirection(json.getVec3f("direction", {0.0f, -1.0f, 0.0f}));

    return component;
}

JsonValue&    ComponentFactory<sLightComponent>::saveToJson(const std::string& entityType, const sComponent* savedComponent, JsonValue* toJson)
{
    JsonValue& json = toJson ? *toJson : _componentsJson[entityType];
    const sLightComponent* component = static_cast<const sLightComponent*>(savedComponent ? savedComponent : _components[entityType]);

    json.setVec3f("ambient", component->light.getAmbient());
    json.setVec3f("diffuse", component->light.getDiffuse());
    json.setVec3f("direction", component->light.getDirection());

    return (json);
}

bool    ComponentFactory<sLightComponent>::updateEditor(const std::string& entityType, sComponent** savedComponent, sComponent* entityComponent, Entity* entity)
{
    sLightComponent* component = static_cast<sLightComponent*>(entityComponent ? entityComponent : _components[entityType]);
    *savedComponent = component;
    bool changed = false;

    // Ambient
    {
        glm::vec3 ambient = component->light.getAmbient();
        if (ImGui::ColorEdit3("Ambient", glm::value_ptr(ambient)))
        {
            component->light.setAmbient(ambient);
            changed  = true;
        }
    }

    // Diffuse
    {
        glm::vec3 diffuse = component->light.getDiffuse();
        if (ImGui::ColorEdit3("Diffuse", glm::value_ptr(diffuse)))
        {
            component->light.setDiffuse(diffuse);
            changed  = true;
        }
    }

    return (changed);
}
