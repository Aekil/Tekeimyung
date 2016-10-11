#pragma once

#include <utility>
#include "Utils/Exception.hpp"
#include "json/json.h"
#include "Core/Components.hh"

// Generate map initializer lists
// ex: { "sRenderComponent", sRenderComponent }, { "sPositionComponent", sPositionComponent }
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


/*
** IComponentFactory
*/

class IComponentFactory
{
public:
    IComponentFactory() {}
    virtual ~IComponentFactory() {}
    virtual sComponent* loadFromJson(const std::string& entityType, Json::Value& json) = 0;
    static bool                                                     componentTypeExists(const std::string& type);
    static eOrientation                                             stringToOrientation(const std::string& orientationStr);
    static void                                                     initComponent(const std::string& entityType, const std::string& name, Json::Value& value);
    static sComponent*                                              createComponent(const std::string& entityType, const std::string& name);
    virtual sComponent*                                             clone(const std::string& entityType) = 0;
    virtual void                                                    addComponent(const std::string& entityType, sComponent* component) = 0;

private:
    // Store Components types
    static std::unordered_map<std::string, IComponentFactory*>      _componentsTypes;
};


/*
** BaseComponentFactory
*/

template <typename T>
class BaseComponentFactory: public IComponentFactory
{
public:
    BaseComponentFactory() {}
    ~BaseComponentFactory() {}

    sComponent* loadFromJson(const std::string& entityType, Json::Value& json)
    {
        EXCEPT(NotImplementedException, "Failed to load component: the component has no overload");
    }

    void addComponent(const std::string& entityType, sComponent* component)
    {
        _components[entityType] = component;
    }

private:
    sComponent*         clone(const std::string& entityType)
    {
        return _components[entityType]->clone();
    }

protected:
    // One component per entity type
    std::unordered_map<std::string, sComponent*>    _components;
};


/*
** ComponentFactory
*/
template <typename T>
class ComponentFactory
{
public:
    ComponentFactory() {}
    ~ComponentFactory() {}
};

/*
** sRenderComponent
*/

template <>
class ComponentFactory<sRenderComponent>: public BaseComponentFactory<sRenderComponent>
{
public:
    sComponent* loadFromJson(const std::string& entityType, Json::Value& json);

private:
    Sprite::eType stringToSpriteType(const std::string& spriteTypeStr);
};


/*
** sPositionComponent
*/

template <>
class ComponentFactory<sPositionComponent>: public BaseComponentFactory<sPositionComponent>
{
public:
    sComponent* loadFromJson(const std::string& entityType, Json::Value& json);
};


/*
** sInputComponent
*/

template <>
class ComponentFactory<sInputComponent>: public BaseComponentFactory<sInputComponent>
{
public:
    sComponent* loadFromJson(const std::string& entityType, Json::Value& json);
};


/*
** sDirectionComponent
*/

template <>
class ComponentFactory<sDirectionComponent>: public BaseComponentFactory<sDirectionComponent>
{
public:
    sComponent* loadFromJson(const std::string& entityType, Json::Value& json);
};


/*
** sHitBoxComponent
*/

template <>
class ComponentFactory<sHitBoxComponent>: public BaseComponentFactory<sHitBoxComponent>
{
public:
    sComponent* loadFromJson(const std::string& entityType, Json::Value& json);
};


/*
** sCircleHitBoxComponent
*/

template <>
class ComponentFactory<sCircleHitBoxComponent>: public BaseComponentFactory<sCircleHitBoxComponent>
{
public:
    sComponent* loadFromJson(const std::string& entityType, Json::Value& json);
};


/*
** sGravityComponent
*/

template <>
class ComponentFactory<sGravityComponent>: public BaseComponentFactory<sGravityComponent>
{
public:
    sComponent* loadFromJson(const std::string& entityType, Json::Value& json);
};


/*
** sTypeComponent
*/

template <>
class ComponentFactory<sTypeComponent>: public BaseComponentFactory<sTypeComponent>
{
public:
    sComponent* loadFromJson(const std::string& entityType, Json::Value& json);
    eEntityType stringToEntityType(const std::string& entityTypeStr);
};
