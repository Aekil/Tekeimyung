#include <sstream>
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
    }
    catch(const std::exception& e)
    {
        std::stringstream msg;

        msg << "Failed to load " << name << ": " << e.what();
        EXCEPT(InternalErrorException, msg.str());
    }
}

sComponent*  IComponentFactory::createComponent(const std::string& entityType, const std::string& name)
{
    return _componentsTypes[name]->clone(entityType);
}

/*
** sRenderComponent
*/

sComponent* ComponentFactory<sRenderComponent>::loadFromJson(const std::string& entityType, Json::Value& json)
{
    sRenderComponent* component = new sRenderComponent();

    component->texture = json.get("texture", "").asString();
    component->type = stringToSpriteType(json.get("type", "").asString());
    component->animated = json.get("animated", false).asBool();
    component->nbFrames = json.get("nbFrames", 0).asInt();
    component->nbFrames = json.get("nbFrames", 0).asInt();
    component->spriteSize.x = json.get("spriteSize", {}).get("width", 0).asFloat();
    component->spriteSize.y = json.get("spriteSize", {}).get("height", 0).asFloat();

    for (auto&& orientation: json.get("orientations", "[]"))
    {
        component->orientations.push_back(stringToOrientation(orientation.asString()));
    }

    return component;
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
    component->orientation =  stringToOrientation(json.get("orientation", "S").asString());

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
    else if (entityTypeStr == "TILE")
        return eEntityType::TILE;
    else if (entityTypeStr == "TILE_STAIRS_UP")
        return eEntityType::TILE_STAIRS_UP;
    else if (entityTypeStr == "TILE_STAIRS_DOWN")
        return eEntityType::TILE_STAIRS_DOWN;

    EXCEPT(NotImplementedException, "Failed to load sTypeComponent:  the entity type does not exist");
}

sComponent* ComponentFactory<sTypeComponent>::loadFromJson(const std::string& entityType, Json::Value& json)
{
    sTypeComponent* component = new sTypeComponent();

    component->type = stringToEntityType(json.get("type", "TILE").asString());

    return component;
}
