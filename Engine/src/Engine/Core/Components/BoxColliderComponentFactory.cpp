/**
* @Author   Guillaume Labey
*/

#include <glm/gtc/type_ptr.hpp>

#include <Engine/Core/Components/RenderComponent.hh>

#include <Engine/Core/Components/BoxColliderComponentFactory.hpp>

sComponent* ComponentFactory<sBoxColliderComponent>::loadFromJson(const std::string& entityType, const JsonValue& json)
{
    sBoxColliderComponent* component = new sBoxColliderComponent();

    component->pos = json.getVec3f("pos", { 0.0f, 0.0f, 0.0f });
    component->size = json.getVec3f("size", { 2.0f, 2.0f, 2.0f });
    component->isTrigger = json.getBool("isTrigger", false);

    return (component);
}

JsonValue&    ComponentFactory<sBoxColliderComponent>::saveToJson(const std::string& entityType, const sComponent* savedComponent, JsonValue* toJson)
{
    JsonValue& json = toJson ? *toJson : _componentsJson[entityType];
    const sBoxColliderComponent* component = static_cast<const sBoxColliderComponent*>(savedComponent ? savedComponent : _components[entityType]);

    json.setVec3f("pos", component->pos);
    json.setVec3f("size", component->size);
    json.setBool("isTrigger", component->isTrigger);

    return (json);
}

bool    ComponentFactory<sBoxColliderComponent>::updateEditor(const std::string& entityType, sComponent** savedComponent, sComponent* entityComponent, Entity* entity)
{
    sBoxColliderComponent* component = static_cast<sBoxColliderComponent*>(entityComponent ? entityComponent : _components[entityType]);
    *savedComponent = component;
    bool changed = false;

    if (ImGui::Button("Reset"))
    {
        sRenderComponent* render = entity->getComponent<sRenderComponent>();
        if (render)
        {
            component->size.x = (render->getModel()->getSize().x + 1.0f) / SIZE_UNIT;
            component->size.y = (render->getModel()->getSize().y + 1.0f) / SIZE_UNIT;
            component->size.z = (render->getModel()->getSize().z + 1.0f) / SIZE_UNIT;
            component->pos = glm::vec3(render->getModel()->getMin().x + (render->getModel()->getSize().x / 2.0f),
                                        render->getModel()->getMin().y + (render->getModel()->getSize().y / 2.0f),
                                        render->getModel()->getMin().z + (render->getModel()->getSize().z / 2.0f));
        }
    }
    if (ImGui::Button(component->display ? "Hide" : "Display"))
    {
        component->display = !component->display;
    }

    if (ImGui::Button(component->isTrigger ? "Trigger" : "No trigger"))
    {
        component->isTrigger = !component->isTrigger;
    }

    ImGui::InputFloat3("position", glm::value_ptr(component->pos), 3);
    ImGui::InputFloat3("size", glm::value_ptr(component->size), 3);

    return (changed);
}
