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

    EXCEPT(NotImplementedException, "Failed to load sRenderComponent:  the orientation %s does not exist", orientationStr);
}

std::string IComponentFactory::orientationToString(eOrientation orientation)
{
    if (orientation == eOrientation::N)
        return ("N");
    else if (orientation == eOrientation::NE)
        return ("NE");
    else if (orientation == eOrientation::E)
        return ("E");
    else if (orientation == eOrientation::SE)
        return ("SE");
    else if (orientation == eOrientation::S)
        return ("S");
    else if (orientation == eOrientation::SW)
        return ("SW");
    else if (orientation == eOrientation::W)
        return ("W");
    else if (orientation == eOrientation::NW)
        return ("NW");

    EXCEPT(NotImplementedException, "Failed to save sRenderComponent:  the orientation type does not exist");
}

void    IComponentFactory::initComponent(const std::string& entityType, const std::string& name, const JsonValue& value)
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

sComponent* ComponentFactory<sRenderComponent>::loadFromJson(const std::string& entityType, const JsonValue& json)
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
    if (json.getBool("animated", false) && animation.size() > 0)
    {
        component->animated = true;
        component->frames = animation.getUVec2f("frames", { 0, 0 });
        component->spriteSheetOffset = animation.getVec2f("offset", { 0.0f, 0.0f });
        for (auto &&orientation: animation.get("orientations", {}).get())
        {
            component->orientations.push_back(stringToOrientation(orientation.asString()));
        }
    }

    return (component);
}

JsonValue&    ComponentFactory<sRenderComponent>::saveToJson(const std::string& entityType, const std::string& componentType)
{
    std::vector<std::string> orientations;
    JsonValue animation;
    JsonValue& json = _componentsJson[entityType];
    sRenderComponent* component = static_cast<sRenderComponent*>(_components[entityType]);


    // Write animation
    animation.setUVec2f("frames", component->frames);
    animation.setVec2f("offset", component->spriteSheetOffset);
    // Convert orientations into string
    for (auto &&orientation: component->orientations)
    {
        orientations.push_back(orientationToString(orientation));
    }
    animation.setStringVec("orientations", orientations);
    json.setValue("animation", animation);
    json.setBool("animated", component->animated);

    json.setString("texture", component->texture);
    json.setString("type", spriteTypeToString(component->type));
    json.setVec2f("spriteSize", component->spriteSize);
    json.setColor3f("color", component->color);

    return (json);
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

    EXCEPT(NotImplementedException, "Failed to load sRenderComponent:  the sprite type %s does not exist", spriteTypeStr);
}

std::string ComponentFactory<sRenderComponent>::spriteTypeToString(Sprite::eType spriteType)
{
    if (spriteType == Sprite::eType::OBJECT)
        return ("OBJECT");
    else if (spriteType == Sprite::eType::TILE)
        return ("TILE");

    EXCEPT(NotImplementedException, "Failed to save sRenderComponent:  the sprite type does not exist");
}


/*
** sPositionComponent
*/

sComponent* ComponentFactory<sPositionComponent>::loadFromJson(const std::string& entityType, const JsonValue& json)
{
    sPositionComponent* component = new sPositionComponent();

    component->value.x = json.getFloat("x", 0.0f);
    component->value.y = json.getFloat("y", 0.0f);
    component->z = json.getFloat("z", 0.0f);

    return (component);
}

JsonValue&    ComponentFactory<sPositionComponent>::saveToJson(const std::string& entityType, const std::string& componentType)
{
    JsonValue& json = _componentsJson[entityType];
    sPositionComponent* component = static_cast<sPositionComponent*>(_components[entityType]);

    json.setFloat("x", component->value.x);
    json.setFloat("y", component->value.y);
    json.setFloat("z", component->z);

    return (json);
}


/*
** sInputComponent
*/

sComponent* ComponentFactory<sInputComponent>::loadFromJson(const std::string& entityType, const JsonValue& json)
{
    Keyboard keyboard;
    auto &&keyMap = keyboard.getStringMap();
    sInputComponent* component = new sInputComponent();

    component->moveLeft = keyMap[json.getString("left", "Q")];
    component->moveRight = keyMap[json.getString("right", "D")];
    component->moveUp = keyMap[json.getString("up", "Z")];
    component->moveDown = keyMap[json.getString("down", "S")];

    return (component);
}

JsonValue&    ComponentFactory<sInputComponent>::saveToJson(const std::string& entityType, const std::string& componentType)
{
    JsonValue& json = _componentsJson[entityType];
    sInputComponent* component = static_cast<sInputComponent*>(_components[entityType]);
    Keyboard keyboard;

    json.setString("left", keyboard.keyToString(component->moveLeft));
    json.setString("right", keyboard.keyToString(component->moveRight));
    json.setString("up", keyboard.keyToString(component->moveUp));
    json.setString("down", keyboard.keyToString(component->moveDown));

    return (json);
}


/*
** sDirectionComponent
*/

sComponent* ComponentFactory<sDirectionComponent>::loadFromJson(const std::string& entityType, const JsonValue& json)
{
    sDirectionComponent* component = new sDirectionComponent();

    component->value.x = json.getFloat("x", 0.0f);
    component->value.y = json.getFloat("y", 0.0f);
    component->orientation =  stringToOrientation(json.getString("orientation", "N"));
    component->speed =  json.getFloat("speed", 1.0f);

    return (component);
}

JsonValue&    ComponentFactory<sDirectionComponent>::saveToJson(const std::string& entityType, const std::string& componentType)
{
    JsonValue& json = _componentsJson[entityType];
    sDirectionComponent* component = static_cast<sDirectionComponent*>(_components[entityType]);

    json.setFloat("x", component->value.x);
    json.setFloat("y", component->value.y);
    json.setString("orientation", orientationToString(component->orientation));
    json.setFloat("speed", component->speed);

    return (json);
}


/*
** sHitBoxComponent
*/

sComponent* ComponentFactory<sHitBoxComponent>::loadFromJson(const std::string& entityType, const JsonValue& json)
{
    sHitBoxComponent* component = new sHitBoxComponent();

    component->min = json.getVec2f("min", { 0.0f, 0.0f });
    component->max = json.getVec2f("max", { 0.0f, 0.0f });

    return (component);
}

JsonValue&    ComponentFactory<sHitBoxComponent>::saveToJson(const std::string& entityType, const std::string& componentType)
{
    JsonValue& json = _componentsJson[entityType];
    sHitBoxComponent* component = static_cast<sHitBoxComponent*>(_components[entityType]);

    json.setVec2f("min", component->min);
    json.setVec2f("max", component->max);

    return (json);
}


/*
** sCircleHitBoxComponent
*/

sComponent* ComponentFactory<sCircleHitBoxComponent>::loadFromJson(const std::string& entityType, const JsonValue& json)
{
    sCircleHitBoxComponent* component = new sCircleHitBoxComponent();

    component->center = json.getVec2f("center", { 0.0f, 0.0f });
    component->radius = json.getFloat("radius", 0.0f);

    return (component);
}

JsonValue&    ComponentFactory<sCircleHitBoxComponent>::saveToJson(const std::string& entityType, const std::string& componentType)
{
    JsonValue& json = _componentsJson[entityType];
    sCircleHitBoxComponent* component = static_cast<sCircleHitBoxComponent*>(_components[entityType]);

    json.setVec2f("center", component->center);
    json.setFloat("radius", component->radius);

    return (json);
}


/*
** sGravityComponent
*/

sComponent* ComponentFactory<sGravityComponent>::loadFromJson(const std::string& entityType, const JsonValue& json)
{
    sGravityComponent* component = new sGravityComponent();

    component->value.x = json.getFloat("x", 0.0f);
    component->value.y = json.getFloat("y", 0.0f);

    return (component);
}

JsonValue&    ComponentFactory<sGravityComponent>::saveToJson(const std::string& entityType, const std::string& componentType)
{
    JsonValue& json = _componentsJson[entityType];
    sGravityComponent* component = static_cast<sGravityComponent*>(_components[entityType]);

    json.setFloat("x", component->value.x);
    json.setFloat("y", component->value.y);

    return (json);
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

    EXCEPT(NotImplementedException, "Failed to load sTypeComponent:  the entity type %s does not exist", entityTypeStr);
}

std::string ComponentFactory<sTypeComponent>::entityTypeToString(eEntityType entityType)
{
    if (entityType == eEntityType::CHARACTER)
        return ("CHARACTER");
    else if (entityType == eEntityType::OBJECT)
        return ("OBJECT");
    else if (entityType == eEntityType::TILE_WALKABLE)
        return ("TILE_WALKABLE");
    else if (entityType == eEntityType::TILE_NOT_WALKABLE)
        return ("TILE_NOT_WALKABLE");
    else if (entityType == eEntityType::TILE_STAIRS_UP)
        return ("TILE_STAIRS_UP");
    else if (entityType == eEntityType::TILE_STAIRS_DOWN)
        return ("TILE_STAIRS_DOWN");

    EXCEPT(NotImplementedException, "Failed to save sTypeComponent:  the entity type does not exist");
}


JsonValue&    ComponentFactory<sTypeComponent>::saveToJson(const std::string& entityType, const std::string& componentType)
{
    JsonValue& json = _componentsJson[entityType];
    sTypeComponent* component = static_cast<sTypeComponent*>(_components[entityType]);

    json.setString("type", entityTypeToString(component->type));

    return (json);
}

sComponent* ComponentFactory<sTypeComponent>::loadFromJson(const std::string& entityType, const JsonValue& json)
{
    sTypeComponent* component = new sTypeComponent();

    component->type = stringToEntityType(json.getString("type", "TILE_WALKABLE"));

    return (component);
}


/*
** sAIComponent
*/

sComponent* ComponentFactory<sAIComponent>::loadFromJson(const std::string& entityType, const JsonValue& json)
{
    sAIComponent* component = new sAIComponent();

    return (component);
}


/*
** sParticleEmitterComponent
*/

sComponent* ComponentFactory<sParticleEmitterComponent>::loadFromJson(const std::string& entityType, const JsonValue& json)
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

    return (component);
}

JsonValue&    ComponentFactory<sParticleEmitterComponent>::saveToJson(const std::string& entityType, const std::string& componentType)
{
    JsonValue color;
    JsonValue size;
    JsonValue& json = _componentsJson[entityType];
    sParticleEmitterComponent* component = static_cast<sParticleEmitterComponent*>(_components[entityType]);

    // Write colors
    color.setColor4f("start", component->colorStart);
    color.setColor4f("start_variance", component->colorStartVariance);
    color.setColor4f("finish", component->colorFinish);
    color.setColor4f("finish_variance", component->colorFinishVariance);
    json.setValue("color", color);

    // Write size
    size.setFloat("start", component->sizeStart);
    size.setFloat("start_variance", component->sizeStartVariance);
    size.setFloat("finish", component->sizeFinish);
    size.setFloat("finish_variance", component->sizeFinishVariance);
    json.setValue("size", size);

    json.setFloat("rate", component->rate);
    json.setUInt("spawn_nb", component->spawnNb);
    json.setUInt("life", component->life);
    json.setUInt("life_variance", component->lifeVariance);
    json.setFloat("angle", component->angle);
    json.setFloat("angle_variance", component->angleVariance);
    json.setFloat("speed", component->speed);
    json.setFloat("speed_variance", component->speedVariance);

    return (json);
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
        changed |= ImGui::SliderInt("Particles number per spawn", (int*)&component->spawnNb, 0, 50);
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
    }

    return (changed);
}
