/**
* @Author   Guillaume Labey
*/

#include <glm/gtc/type_ptr.hpp>

#include <Engine/Core/Components/RenderComponentFactory.hpp>

#include <Engine/Core/Components/ParticleEmitterComponentFactory.hpp>

constexpr unsigned int sParticleEmitterComponent::identifier;

sComponent* ComponentFactory<sParticleEmitterComponent>::loadFromJson(const std::string& entityType, const JsonValue& json)
{
    sParticleEmitterComponent* component = new sParticleEmitterComponent();
    JsonValue color(json.get("color", {}));
    JsonValue size = json.get("size", {});

    component->rate = json.getFloat("rate", 0.0f);
    component->maxParticles = json.getUInt("max_particles", 50);
    component->emitterLife = json.getFloat("emitter_life", 0.0f);
    component->life = json.getUInt("life", 80);
    component->lifeVariance = json.getUInt("life_variance", 0);
    component->angle = json.getFloat("angle", 50.0f);
    component->angleVariance = json.getFloat("angle_variance", 0.0f);
    component->speed = json.getFloat("speed", 30.0f);
    component->speedVariance =  json.getFloat("speed_variance", 0.0f);

    if (color.size() > 0)
    {
        component->colorStart = color.getColor4f("start", { 1.0f, 1.0f, 1.0f, 1.0f });
        component->colorFinish = color.getColor4f("finish", { 1.0f, 1.0f, 1.0f, 1.0f });
    }
    else
    {
        component->colorStart = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        component->colorFinish = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
    }

    if (size.size() > 0)
    {
        component->sizeStart =  size.getFloat("start", 1.0f);
        component->sizeFinish =  size.getFloat("finish", 1.0f);
        component->sizeStartVariance =  size.getFloat("start_variance", 1.0f);
        component->sizeFinishVariance =  size.getFloat("finish_variance", 1.0f);
    }
    else
    {
        component->sizeStart = 1.0f;
        component->sizeFinish = 0.0f;
        component->sizeStartVariance = 0.0f;
        component->sizeFinishVariance = 0.0f;
    }

    component->displayOnlyParticles = json.getBool("display_only_particles", true);
    component->modelFile = json.getString("model", "resources/models/default.DAE");

    component->type = EnumManager<Geometry::eType>::stringToEnum(json.getString("type", "MESH"));

    ComponentFactory<sRenderComponent>::loadModelMaterialsFromJson(entityType, component->getModelInstance(), json);

    return (component);
}

JsonValue&    ComponentFactory<sParticleEmitterComponent>::saveToJson(const std::string& entityType, const sComponent* savedComponent, JsonValue* toJson)
{
    JsonValue color;
    JsonValue size;
    JsonValue& json = toJson ? *toJson : _componentsJson[entityType];
    const sParticleEmitterComponent* component = static_cast<const sParticleEmitterComponent*>(savedComponent ? savedComponent : _components[entityType]);

    // Write colors
    color.setColor4f("start", component->colorStart);
    color.setColor4f("finish", component->colorFinish);
    json.setValue("color", color);

    // Write size
    size.setFloat("start", component->sizeStart);
    size.setFloat("start_variance", component->sizeStartVariance);
    size.setFloat("finish", component->sizeFinish);
    size.setFloat("finish_variance", component->sizeFinishVariance);
    json.setValue("size", size);

    json.setFloat("rate", component->rate);
    json.setUInt("max_particles", component->maxParticles);
    json.setFloat("emitter_life", component->emitterLife);
    json.setUInt("life", component->life);
    json.setUInt("life_variance", component->lifeVariance);
    json.setFloat("angle", component->angle);
    json.setFloat("angle_variance", component->angleVariance);
    json.setFloat("speed", component->speed);
    json.setFloat("speed_variance", component->speedVariance);

    json.setBool("display_only_particles", component->displayOnlyParticles);
    json.setString("model", component->modelFile);
    json.setString("type", EnumManager<Geometry::eType>::enumToString(component->type));
    ComponentFactory<sRenderComponent>::saveModelMaterialsFromJson(const_cast<sParticleEmitterComponent*>(component)->getModelInstance(), json);

    return (json);
}

bool    ComponentFactory<sParticleEmitterComponent>::updateEditor(const std::string& entityType, sComponent** savedComponent, sComponent* entityComponent, Entity* entity)
{
    sParticleEmitterComponent* component = static_cast<sParticleEmitterComponent*>(entityComponent ? entityComponent : _components[entityType]);
    *savedComponent = component;
    bool changed = false;

    changed |= ImGui::Checkbox("Display only particles", &component->displayOnlyParticles);
    if (Helper::updateComboEnum<Geometry::eType>("Model type", component->type))
    {
        component->_modelInstance = nullptr;
    }

    if (component->type == Geometry::eType::MESH)
    {
        std::string modelName = component->getModel()->getId();
        if (Helper::updateComboString("model", ResourceManager::getInstance()->getResourcesNames<Model>(), modelName))
        {
            component->_modelInstance = nullptr;
            auto model = ResourceManager::getInstance()->getOrLoadResource<Model>(modelName);
            component->modelFile = model->getPath();
        }
    }
    ComponentFactory<sRenderComponent>::updateMaterialsEditor(component->getModelInstance());
    ImGui::Text("\n");

    //changed |= ImGui::InputText("Texture string", &(component->texture[0]), sizeof(char*));
    changed |= ImGui::SliderFloat("Emitter life (0 to disable)", &component->emitterLife, 0.0f, 10.0f);
    changed |= ImGui::SliderFloat("Rate", &component->rate, 0.0f, 3.0f);
    changed |= ImGui::SliderInt("Max particles", (int*)&component->maxParticles, 0, 500);
    changed |= ImGui::SliderFloat("Angle", &component->angle, 0.0f, 360.0f);
    changed |= ImGui::SliderFloat("Angle variance", &component->angleVariance, 0.0f, 360.0f);
    changed |= ImGui::SliderFloat("Speed", &component->speed, 0.0f, 200.0f);
    changed |= ImGui::SliderFloat("Speed variance", &component->speedVariance, 0.0f, 200.0f);
    changed |= ImGui::SliderInt("Life", (int*)&component->life, 0, 200);
    changed |= ImGui::SliderInt("Life variance", (int*)&component->lifeVariance, 0, 200);
    changed |= ImGui::ColorEdit4("Start color", glm::value_ptr(component->colorStart));
    changed |= ImGui::ColorEdit4("Finish color", glm::value_ptr(component->colorFinish));
    changed |= ImGui::SliderFloat("Start size", &component->sizeStart, 0.0f, 5.0f);
    changed |= ImGui::SliderFloat("Finish size", &component->sizeFinish, 0.0f, 5.0f);
    changed |= ImGui::SliderFloat("Start size variance", &component->sizeStartVariance, 0.0f, 5.0f);
    changed |= ImGui::SliderFloat("Finish size variance", &component->sizeFinishVariance, 0.0f, 5.0f);

    return (changed);
}
