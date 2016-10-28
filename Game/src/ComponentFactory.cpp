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

void    IComponentFactory::initComponent(const std::string& entityType, const std::string& name, Json::Value& value)
{
    try
    {
        sComponent* component;
        component = _componentsTypes[name]->loadFromJson(entityType, value);
        _componentsTypes[name]->addComponent(entityType, component);
        _componentsTypes[name]->saveComponentJson(entityType, value);
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

sComponent* ComponentFactory<sRenderComponent>::loadFromJson(const std::string& entityType, Json::Value& json)
{
    sRenderComponent* component = new sRenderComponent();
    Json::Value animation = json.get("animation", {});

    // Initialize some values
    component->animated = false;
    component->spriteSheetOffset = {0, 0};

    // Sprite texture
    component->texture = json.get("texture", "").asString();

    // Sprite type
    component->type = stringToSpriteType(json.get("type", "").asString());

    // Sprite size
    component->spriteSize.x = json.get("spriteSize", {}).get("width", 0).asFloat();
    component->spriteSize.y = json.get("spriteSize", {}).get("height", 0).asFloat();

    // Sprite color
    component->color.x = json.get("color", {}).get("r", 1).asFloat();
    component->color.y = json.get("color", {}).get("g", 1).asFloat();
    component->color.z = json.get("color", {}).get("b", 1).asFloat();

    // Sprite animation
    if (animation.size() > 0)
    {
        component->animated = true;
        component->frames.x = animation.get("frames", {}).get("x", 0).asFloat();
        component->frames.y = animation.get("frames", {}).get("y", 0).asFloat();
        component->spriteSheetOffset.x = animation.get("offset", {}).get("x", 0).asFloat();
        component->spriteSheetOffset.y = animation.get("offset", {}).get("y", 0).asFloat();
        for (auto&& orientation: animation.get("orientations", "[]"))
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

sComponent* ComponentFactory<sPositionComponent>::loadFromJson(const std::string& entityType, Json::Value& json)
{
    sPositionComponent* component = new sPositionComponent();

    component->value.x = json.get("x", 0).asFloat();
    component->value.y = json.get("y", 0).asFloat();
    component->z = json.get("z", 0).asFloat();

    return component;
}


/*
** sInputComponent
*/

sComponent* ComponentFactory<sInputComponent>::loadFromJson(const std::string& entityType, Json::Value& json)
{
    Keyboard keyboard;
    auto &&keyMap = keyboard.getStringMap();
    sInputComponent* component = new sInputComponent();

    component->moveLeft = keyMap[json.get("left", "Q").asString()];
    component->moveRight = keyMap[json.get("right", "D").asString()];
    component->moveUp = keyMap[json.get("up", "Z").asString()];
    component->moveDown = keyMap[json.get("down", "S").asString()];

    return component;
}


/*
** sDirectionComponent
*/

sComponent* ComponentFactory<sDirectionComponent>::loadFromJson(const std::string& entityType, Json::Value& json)
{
    sDirectionComponent* component = new sDirectionComponent();

    component->value.x = json.get("x", 0).asFloat();
    component->value.y = json.get("y", 0).asFloat();
    component->orientation =  stringToOrientation(json.get("orientation", "N").asString());
    component->speed =  json.get("speed", 1.0f).asFloat();

    return component;
}


/*
** sHitBoxComponent
*/

sComponent* ComponentFactory<sHitBoxComponent>::loadFromJson(const std::string& entityType, Json::Value& json)
{
    sHitBoxComponent* component = new sHitBoxComponent();

    component->min.x =  json.get("min", {}).get("x", 0).asFloat();
    component->min.y =  json.get("min", {}).get("y", 0).asFloat();
    component->max.x =  json.get("max", {}).get("x", 0).asFloat();
    component->max.y =  json.get("max", {}).get("y", 0).asFloat();

    return component;
}


/*
** sCircleHitBoxComponent
*/

sComponent* ComponentFactory<sCircleHitBoxComponent>::loadFromJson(const std::string& entityType, Json::Value& json)
{
    sCircleHitBoxComponent* component = new sCircleHitBoxComponent();

    component->center.x =  json.get("center", {}).get("x", 0).asFloat();
    component->center.y = json.get("center", {}).get("y", 0).asFloat();
    component->radius = json.get("radius", 0).asFloat();

    return component;
}


/*
** sGravityComponent
*/

sComponent* ComponentFactory<sGravityComponent>::loadFromJson(const std::string& entityType, Json::Value& json)
{
    sGravityComponent* component = new sGravityComponent();

    component->value.x = json.get("x", 0).asFloat();
    component->value.y = json.get("y", 0).asFloat();

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

sComponent* ComponentFactory<sTypeComponent>::loadFromJson(const std::string& entityType, Json::Value& json)
{
    sTypeComponent* component = new sTypeComponent();

    component->type = stringToEntityType(json.get("type", "TILE_WALKABLE").asString());

    return component;
}


/*
** sAIComponent
*/

sComponent* ComponentFactory<sAIComponent>::loadFromJson(const std::string& entityType, Json::Value& json)
{
    sAIComponent* component = new sAIComponent();

    return component;
}


/*
** sParticleEmitterComponent
*/

sComponent* ComponentFactory<sParticleEmitterComponent>::loadFromJson(const std::string& entityType, Json::Value& json)
{
    sParticleEmitterComponent* component = new sParticleEmitterComponent();
    Json::Value color = json.get("color", {});
    Json::Value size = json.get("size", {});

    component->rate = json.get("rate", "").asFloat();
    component->spawnNb = json.get("spawn_nb", "").asInt();
    component->life = json.get("life", "").asInt();
    component->lifeVariance = json.get("life_variance", "").asInt();
    component->angle = json.get("angle", "").asFloat();
    component->angleVariance = json.get("angle_variance", "").asFloat();
    component->speed = json.get("speed", "").asFloat();
    component->speedVariance =  json.get("speed_variance", 0.0f).asFloat();

    if (color.size() > 0)
    {
        Json::Value colorStart = color.get("start", {});
        Json::Value colorStartVariance = color.get("start_variance", {});
        Json::Value colorFinish = color.get("finish", {});
        Json::Value colorFinishVariance = color.get("finish_variance", {});

        component->colorStart = { colorStart.get("r", 1).asFloat(), colorStart.get("g", 1).asFloat(),
        colorStart.get("b", 1).asFloat(), 1 };
        component->colorStartVariance = { colorStartVariance.get("r", 1).asFloat(), colorStartVariance.get("g", 1).asFloat(),
        colorStartVariance.get("b", 1).asFloat(), colorStartVariance.get("a", 1).asFloat() };
        component->colorFinish = { colorFinish.get("r", 1).asFloat(), colorFinish.get("g", 1).asFloat(),
        colorFinish.get("b", 1).asFloat(), colorFinish.get("a", 1).asFloat() };
        component->colorStartVariance = { colorStartVariance.get("r", 1).asFloat(), colorStartVariance.get("g", 1).asFloat(),
        colorStartVariance.get("b", 1).asFloat(), colorStartVariance.get("a", 1).asFloat() };
    }

    if (size.size() > 0)
    {
        component->sizeStart =  size.get("start", 1.0f).asFloat();
        component->sizeFinish =  size.get("finish", 1.0f).asFloat();
        component->sizeStartVariance =  size.get("start_variance", 1.0f).asFloat();
        component->sizeFinishVariance =  size.get("finish_variance", 1.0f).asFloat();
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
