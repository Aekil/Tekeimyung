#pragma once

#include "Utils/Exception.hpp"
#include "json/json.h"
#include "Core/Components.hh"


#define COMPONENTS_TYPES()\
    GENERATE_PAIRS(sRenderComponent),\
    GENERATE_PAIRS(sPositionComponent),\
    GENERATE_PAIRS(sInputComponent),\
    GENERATE_PAIRS(sDirectionComponent),\
    GENERATE_PAIRS(sHitBoxComponent),\
    GENERATE_PAIRS(sCircleHitBoxComponent),\
    GENERATE_PAIRS(sGravityComponent),\
    GENERATE_PAIRS(sTypeComponent),\

#define GENERATE_PAIRS(COMPONENT) { #COMPONENT, new ComponentFactory<COMPONENT>() }

class IComponentFactory
{
public:
    IComponentFactory() {}
    virtual ~IComponentFactory() {}
    virtual sComponent* loadFromJson(Json::Value& json) = 0;
    static bool                                                     componentTypeExists(const std::string& type);
    static eOrientation                                             stringToOrientation(const std::string& orientationStr);
    static sComponent*                                              createComponent(const std::string& name, Json::Value& value);

private:
    static std::unordered_map<std::string, IComponentFactory*>      _componentsTypes;
};

template <typename T>
class ComponentFactory: public IComponentFactory
{
public:
    ComponentFactory() {}
    ~ComponentFactory() {}
    sComponent* loadFromJson(Json::Value& json)
    {
        EXCEPT(NotImplementedException, "Failed to load component: the component has no overload");
    }
};

template <>
class ComponentFactory<sRenderComponent>: public IComponentFactory
{
public:
    sComponent* loadFromJson(Json::Value& json);

private:
    Sprite::eType stringToSpriteType(const std::string& spriteTypeStr);
};

template <>
class ComponentFactory<sPositionComponent>: public IComponentFactory
{
public:
    sComponent* loadFromJson(Json::Value& json);
};

template <>
class ComponentFactory<sInputComponent>: public IComponentFactory
{
public:
    sComponent* loadFromJson(Json::Value& json);
};

template <>
class ComponentFactory<sDirectionComponent>: public IComponentFactory
{
public:
    sComponent* loadFromJson(Json::Value& json);
};

template <>
class ComponentFactory<sHitBoxComponent>: public IComponentFactory
{
public:
    sComponent* loadFromJson(Json::Value& json);
};

template <>
class ComponentFactory<sCircleHitBoxComponent>: public IComponentFactory
{
public:
    sComponent* loadFromJson(Json::Value& json);
};

template <>
class ComponentFactory<sGravityComponent>: public IComponentFactory
{
public:
    sComponent* loadFromJson(Json::Value& json);
};

template <>
class ComponentFactory<sTypeComponent>: public IComponentFactory
{
public:
    sComponent* loadFromJson(Json::Value& json);
    eEntityType stringToEntityType(const std::string& entityTypeStr);
};
