#include <sstream>
#include <imgui.h>
#include <imgui_impl_glfw_gl3.h>
#include <glm/gtc/type_ptr.hpp>
#include "Window/Keyboard.hpp"

#include "ComponentFactory.hpp"

std::unordered_map<std::string, IComponentFactory*>  IComponentFactory::_componentsTypes = { COMPONENTS_TYPES() };


/*
** IComponentFactory
*/

bool    IComponentFactory::componentTypeExists(const std::string& type)
{
    for (auto &&componentType: _componentsTypes)
    {
        if (componentType.first == type)
            return (true);
    }

    return (false);
}

eOrientation IComponentFactory::stringToOrientation(const std::string& orientationStr)
{
    if (orientationStr == "N")
        return eOrientation::N;
    else if (orientationStr == "NE")
        return eOrientation::NE;
    else if (orientationStr == "E")
        return eOrientation::E;
    else if (orientationStr == "SE")
        return eOrientation::SE;
    else if (orientationStr == "S")
        return eOrientation::S;
    else if (orientationStr == "SW")
        return eOrientation::SW;
    else if (orientationStr == "W")
        return eOrientation::W;
    else if (orientationStr == "NW")
        return eOrientation::NW;

    EXCEPT(NotImplementedException, "Failed to load sRenderComponent:  the orientation does not exist");
}

void    IComponentFactory::initComponent(const std::string& entityType, const std::string& name, JsonValue& value)
{
    try
    {
        sComponent* component;
        component = _componentsTypes[name]->loadFromJson(entityType, JsonValue(value));
        _componentsTypes[name]->addComponent(entityType, component);
        _componentsTypes[name]->saveComponentJson(entityType, value.get());
    }
    catch(const std::exception& e)
    {
        std::stringstream msg;

        msg << "Failed to load " << name << ": " << e.what();
        EXCEPT(InternalErrorException, msg.str().c_str());
    }
}

sComponent*  IComponentFactory::createComponent(const std::string& entityType, const std::string& name)
{
    return _componentsTypes[name]->clone(entityType);
}

IComponentFactory*   IComponentFactory::getFactory(const std::string& name)
{
    if (!IComponentFactory::componentTypeExists(name))
        EXCEPT(InvalidParametersException, "Cannot get component factory archetype \"%s\": does not exist", name);

    return _componentsTypes[name];
}

/*
** sRenderComponent
*/

sComponent* ComponentFactory<sRenderComponent>::loadFromJson(const std::string& entityType, JsonValue& json)
{
    sRenderComponent* component = new sRenderComponent();
    JsonValue animation = json.get("animation", {});

    // Initialize some values
    component->animated = false;
    component->spriteSheetOffset = {0, 0};

    // Sprite texture
    component->texture = json.getString("texture", "");

    // Sprite type
    component->type = stringToSpriteType(json.getString("type", ""));

    // Sprite size
    component->spriteSize = json.getVec2f("spriteSize", { "width", "height" }, { 0.0f, 0.0f });

    // Sprite color
    component->color = json.getColor3f("color", { 1.0f, 1.0f, 1.0f });

    // Sprite animation
    if (animation.size() > 0)
    {
        component->animated = true;
        component->frames = animation.getVec2f("frames", { 0.0f, 0.0f });
        component->spriteSheetOffset = animation.getVec2f("offset", { 0.0f, 0.0f });
        for (auto &&orientation: animation.get("orientations", {}).get())
        {
            component->orientations.push_back(stringToOrientation(orientation.asString()));
        }
    }

    return component;
}

bool    ComponentFactory<sRenderComponent>::updateEditor(const std::string& entityType, sComponent** component_)
{
    sRenderComponent* component = static_cast<sRenderComponent*>(_components[entityType]);
    *component_ = component;
    bool changed = false;

    ImGui::PushItemWidth(200);
    if (ImGui::CollapsingHeader("sRenderComponent", ImGuiTreeNodeFlags_DefaultOpen))
    {
        changed |= ImGui::ColorEdit3("color", glm::value_ptr(component->color));
    }

    return (changed);
}

Sprite::eType ComponentFactory<sRenderComponent>::stringToSpriteType(const std::string& spriteTypeStr)
{
    if (spriteTypeStr == "OBJECT")
        return Sprite::eType::OBJECT;
    else if (spriteTypeStr == "TILE")
        return Sprite::eType::TILE;

    EXCEPT(NotImplementedException, "Failed to load sRenderComponent:  the sprite type does not exist");
}


/*
** sPositionComponent
*/

sComponent* ComponentFactory<sPositionComponent>::loadFromJson(const std::string& entityType, JsonValue& json)
{
    sPositionComponent* component = new sPositionComponent();

    component->value.x = json.getFloat("x", 0.0f);
    component->value.y = json.getFloat("y", 0.0f);
    component->z = json.getFloat("z", 0.0f);

    return component;
}


/*
** sInputComponent
*/

sComponent* ComponentFactory<sInputComponent>::loadFromJson(const std::string& entityType, JsonValue& json)
{
    Keyboard keyboard;
    auto &&keyMap = keyboard.getStringMap();
    sInputComponent* component = new sInputComponent();

    component->moveLeft = keyMap[json.getString("left", "Q")];
    component->moveRight = keyMap[json.getString("right", "D")];
    component->moveUp = keyMap[json.getString("up", "Z")];
    component->moveDown = keyMap[json.getString("down", "S")];

    return component;
}


/*
** sDirectionComponent
*/

sComponent* ComponentFactory<sDirectionComponent>::loadFromJson(const std::string& entityType, JsonValue& json)
{
    sDirectionComponent* component = new sDirectionComponent();

    component->value.x = json.getFloat("x", 0.0f);
    component->value.y = json.getFloat("y", 0.0f);
    component->orientation =  stringToOrientation(json.getString("orientation", "N"));
    component->speed =  json.getFloat("speed", 1.0f);

    return component;
}


/*
** sHitBoxComponent
*/

sComponent* ComponentFactory<sHitBoxComponent>::loadFromJson(const std::string& entityType, JsonValue& json)
{
    sHitBoxComponent* component = new sHitBoxComponent();

    component->min = json.getVec2f("min", { 0.0f, 0.0f });
    component->max = json.getVec2f("max", { 0.0f, 0.0f });

    return component;
}


/*
** sCircleHitBoxComponent
*/

sComponent* ComponentFactory<sCircleHitBoxComponent>::loadFromJson(const std::string& entityType, JsonValue& json)
{
    sCircleHitBoxComponent* component = new sCircleHitBoxComponent();

    component->center = json.getVec2f("center", { 0.0f, 0.0f });
    component->radius = json.getFloat("radius", 0.0f);

    return component;
}


/*
** sGravityComponent
*/

sComponent* ComponentFactory<sGravityComponent>::loadFromJson(const std::string& entityType, JsonValue& json)
{
    sGravityComponent* component = new sGravityComponent();

    component->value.x = json.getFloat("x", 0.0f);
    component->value.y = json.getFloat("y", 0.0f);

    return component;
}


/*
** sTypeComponent
*/

eEntityType ComponentFactory<sTypeComponent>::stringToEntityType(const std::string& entityTypeStr)
{
    if (entityTypeStr == "CHARACTER")
        return eEntityType::CHARACTER;
    else if (entityTypeStr == "OBJECT")
        return eEntityType::OBJECT;
    else if (entityTypeStr == "TILE_WALKABLE")
        return eEntityType::TILE_WALKABLE;
    else if (entityTypeStr == "TILE_NOT_WALKABLE")
        return eEntityType::TILE_NOT_WALKABLE;
    else if (entityTypeStr == "TILE_STAIRS_UP")
        return eEntityType::TILE_STAIRS_UP;
    else if (entityTypeStr == "TILE_STAIRS_DOWN")
        return eEntityType::TILE_STAIRS_DOWN;

    EXCEPT(NotImplementedException, "Failed to load sTypeComponent:  the entity type does not exist");
}

sComponent* ComponentFactory<sTypeComponent>::loadFromJson(const std::string& entityType, JsonValue& json)
{
    sTypeComponent* component = new sTypeComponent();

    component->type = stringToEntityType(json.getString("type", "TILE_WALKABLE"));

    return component;
}


/*
** sAIComponent
*/

sComponent* ComponentFactory<sAIComponent>::loadFromJson(const std::string& entityType, JsonValue& json)
{
    sAIComponent* component = new sAIComponent();

    return component;
}


/*
** sParticleEmitterComponent
*/

sComponent* ComponentFactory<sParticleEmitterComponent>::loadFromJson(const std::string& entityType, JsonValue& json)
{
    sParticleEmitterComponent* component = new sParticleEmitterComponent();
    JsonValue color(json.get("color", {}));
    JsonValue size = json.get("size", {});

    component->rate = json.getFloat("rate", 0.0f);
    component->spawnNb = json.getUInt("spawn_nb", 0);
    component->life = json.getUInt("life", 0);
    component->lifeVariance = json.getUInt("life_variance", 0);
    component->angle = json.getFloat("angle", 0.0f);
    component->angleVariance = json.getFloat("angle_variance", 0.0f);
    component->speed = json.getFloat("speed", 0.0f);
    component->speedVariance =  json.getFloat("speed_variance", 0.0f);

    if (color.size() > 0)
    {
        component->colorStart = color.getColor4f("start", { 1.0f, 1.0f, 1.0f, 1.0f });
        component->colorStartVariance = color.getColor4f("start_variance", { 1.0f, 1.0f, 1.0f, 1.0f });
        component->colorFinish = color.getColor4f("finish", { 1.0f, 1.0f, 1.0f, 1.0f });
        component->colorFinishVariance = color.getColor4f("finish_variance", { 1.0f, 1.0f, 1.0f, 1.0f });
    }

    if (size.size() > 0)
    {
        component->sizeStart =  size.getFloat("start", 1.0f);
        component->sizeFinish =  size.getFloat("finish", 1.0f);
        component->sizeStartVariance =  size.getFloat("start_variance", 1.0f);
        component->sizeFinishVariance =  size.getFloat("finish_variance", 1.0f);
    }

    return component;
}

Json::Value&    ComponentFactory<sParticleEmitterComponent>::saveToJson(const std::string& entityType, const std::string& componentType)
{
    Json::Value& json = _componentsJson[entityType];
    sParticleEmitterComponent* component = static_cast<sParticleEmitterComponent*>(_components[entityType]);

    // Write colors
    json["color"] = {};
    json["color"]["start_variance"] = {};
    json["color"]["finish"] = {};
    json["color"]["finish_variance"] = {};

    json["color"]["start"] = {};
    json["color"]["start"]["r"] = component->colorStart.x;
    json["color"]["start"]["g"] = component->colorStart.y;
    json["color"]["start"]["b"] = component->colorStart.z;
    json["color"]["start"]["a"] = component->colorStart.w;

    json["color"]["start_variance"] = {};
    json["color"]["start_variance"]["r"] = component->colorStartVariance.x;
    json["color"]["start_variance"]["g"] = component->colorStartVariance.y;
    json["color"]["start_variance"]["b"] = component->colorStartVariance.z;
    json["color"]["start_variance"]["a"] = component->colorStartVariance.w;

    json["color"]["finish"] = {};
    json["color"]["finish"]["r"] = component->colorFinish.x;
    json["color"]["finish"]["g"] = component->colorFinish.y;
    json["color"]["finish"]["b"] = component->colorFinish.z;
    json["color"]["finish"]["a"] = component->colorFinish.w;

    json["color"]["finish_variance"] = {};
    json["color"]["finish_variance"]["r"] = component->colorFinishVariance.x;
    json["color"]["finish_variance"]["g"] = component->colorFinishVariance.y;
    json["color"]["finish_variance"]["b"] = component->colorFinishVariance.z;
    json["color"]["finish_variance"]["a"] = component->colorFinishVariance.w;

    // Write size
    json["size"] = {};
    json["size"]["start"] = component->sizeStart;
    json["size"]["start_variance"] = component->sizeStartVariance;
    json["size"]["finish"] = component->sizeFinish;
    json["size"]["finish"] = component->sizeFinishVariance;

    json["rate"] = component->rate;
    json["spawnNb"] = component->spawnNb;
    json["life_variance"] = component->lifeVariance;
    json["angle"] = component->angle;
    json["angle_variance"] = component->angleVariance;
    json["speed"] = component->speed;
    json["speed_variance"] = component->speedVariance;

    return json;
}

bool    ComponentFactory<sParticleEmitterComponent>::updateEditor(const std::string& entityType, sComponent** component_)
{
    sParticleEmitterComponent* component = static_cast<sParticleEmitterComponent*>(_components[entityType]);
    *component_ = component;
    bool changed = false;

    ImGui::PushItemWidth(200);
    if (ImGui::CollapsingHeader("sParticleEmitterComponent", ImGuiTreeNodeFlags_DefaultOpen))
    {
        changed |= ImGui::SliderFloat("Rate", &component->rate, 0.0f, 3.0f);
        changed |= ImGui::SliderInt("Particles number per spawn", &component->spawnNb, 0.0f, 50.0f);
        changed |= ImGui::SliderFloat("Angle", &component->angle, 0.0f, 360.0f);
        changed |= ImGui::SliderFloat("Angle variance", &component->angleVariance, 0.0f, 360.0f);
        changed |= ImGui::SliderFloat("Speed", &component->speed, 0.0f, 200.0f);
        changed |= ImGui::SliderFloat("Speed variance", &component->speedVariance, 0.0f, 200.0f);
        changed |= ImGui::SliderInt("Life", &component->life, 0.0f, 200.0f);
        changed |= ImGui::SliderInt("Life variance", &component->lifeVariance, 0.0f, 200.0f);
        changed |= ImGui::ColorEdit4("Start color", glm::value_ptr(component->colorStart));
        changed |= ImGui::ColorEdit4("Finish color", glm::value_ptr(component->colorFinish));
        changed |= ImGui::SliderFloat("Start size", &component->sizeStart, 0.0f, 5.0f);
        changed |= ImGui::SliderFloat("Finish size", &component->sizeFinish, 0.0f, 5.0f);
        changed |= ImGui::SliderFloat("Start size variance", &component->sizeStartVariance, 0.0f, 5.0f);
        changed |= ImGui::SliderFloat("Finish size variance", &component->sizeFinishVariance, 0.0f, 5.0f);
    }

    return (changed);
}
