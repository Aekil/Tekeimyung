#include <glm/gtc/type_ptr.hpp>

#include <Engine/Core/Components/RenderComponent.hh>

#include <Engine/Core/Components/SphereColliderComponentFactory.hpp>

sComponent* ComponentFactory<sSphereColliderComponent>::loadFromJson(const std::string& entityType, const JsonValue& json)
{
    sSphereColliderComponent* component = new sSphereColliderComponent();

    component->pos = json.getVec3f("pos", { 0.0f, 0.0f, 0.0f });
    component->radius = json.getFloat("radius", 2.0f);
    component->isTrigger = json.getBool("isTrigger", false);

    return (component);
}

JsonValue&    ComponentFactory<sSphereColliderComponent>::saveToJson(const std::string& entityType, const sComponent* savedComponent, JsonValue* toJson)
{
    JsonValue& json = toJson ? *toJson : _componentsJson[entityType];
    const sSphereColliderComponent* component = static_cast<const sSphereColliderComponent*>(savedComponent ? savedComponent : _components[entityType]);

    json.setVec3f("pos", component->pos);
    json.setFloat("radius", component->radius);
    json.setBool("isTrigger", component->isTrigger);

    return (json);
}

bool    ComponentFactory<sSphereColliderComponent>::updateEditor(const std::string& entityType, sComponent** savedComponent, sComponent* entityComponent, Entity* entity)
{
    sSphereColliderComponent* component = static_cast<sSphereColliderComponent*>(entityComponent ? entityComponent : _components[entityType]);
    *savedComponent = component;
    bool changed = false;

    if (ImGui::Button("Reset"))
    {
        sRenderComponent* render = entity->getComponent<sRenderComponent>();
        if (render)
        {
            float modelMaxSize = std::max({ render->getModel()->getSize().x,
                                            render->getModel()->getSize().y,
                                            render->getModel()->getSize().z });
            component->radius = (modelMaxSize / 2.0f + 1.0f) / (SIZE_UNIT / 2.0f);
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
    ImGui::InputFloat("radius", &component->radius, 3.0f);

    return (changed);
}
