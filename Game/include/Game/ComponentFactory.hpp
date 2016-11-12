#pragma once

#include <utility>

#include <Engine/Utils/Exception.hpp>
#include <Engine/Utils/Logger.hpp>

#include <Game/Utils/JsonValue.hpp>
#include <Game/Components.hh>

// Generate map initializer lists
// ex: { "sRenderComponent", sRenderComponent }, { "sPositionComponent", sPositionComponent }
#define COMPONENTS_TYPES()\
    GENERATE_PAIRS(sRenderComponent),\
    GENERATE_PAIRS(sPositionComponent),\
    GENERATE_PAIRS(sInputComponent),\
    GENERATE_PAIRS(sDirectionComponent),\
    GENERATE_PAIRS(sRectHitboxComponent),\
    GENERATE_PAIRS(sCircleHitboxComponent),\
    GENERATE_PAIRS(sGravityComponent),\
    GENERATE_PAIRS(sTypeComponent),\
    GENERATE_PAIRS(sAIComponent),\
    GENERATE_PAIRS(sParticleEmitterComponent),\
    GENERATE_PAIRS(sTowerAIComponent),\
    GENERATE_PAIRS(sProjectileComponent)\

#define GENERATE_PAIRS(COMPONENT) { #COMPONENT, new ComponentFactory<COMPONENT>() }


/*
** IComponentFactory
*/

class IComponentFactory
{
public:
    IComponentFactory() {}
    virtual ~IComponentFactory() {}

    // Json load/save
    virtual sComponent* loadFromJson(const std::string& entityType, const JsonValue& json) = 0;
    virtual JsonValue& saveToJson(const std::string& entityType, const std::string& componentType) = 0;

    // IComponentFactory methods
    static bool                                                     componentTypeExists(const std::string& type);
    static eOrientation                                             stringToOrientation(const std::string& orientationStr);
    static std::string                                              orientationToString(eOrientation orientation);
    static void                                                     initComponent(const std::string& entityType, const std::string& name, const JsonValue& value);
    static sComponent*                                              createComponent(const std::string& entityType, const std::string& name);
    static IComponentFactory*                                       getFactory(const std::string& name);

    // ComponentFactory overloaded classes methods
    // Ex: ComponentFactory<sPositionComponent>, ComponentFactory<sInputComponent>
    virtual sComponent*                                             clone(const std::string& entityType) = 0;
    virtual void                                                    addComponent(const std::string& entityType, sComponent* component) = 0;
    virtual void                                                    saveComponentJson(const std::string& entityType, const JsonValue& json) = 0;
    virtual bool                                                    updateEditor(const std::string& entityType, sComponent** component_) = 0;

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
    virtual ~BaseComponentFactory() {}

    // loadFromJson has to be overloaded in BaseComponentFactory child classes
    virtual sComponent* loadFromJson(const std::string& entityType, const JsonValue& json)
    {
        EXCEPT(NotImplementedException, "Failed to load component: the component has no overload to load from json");
    }

    // A component can't be saved without this overload
    // In case it's not defined, the loaded component json will be saved
    virtual JsonValue& saveToJson(const std::string& entityType, const std::string& componentType)
    {
        LOG_WARN("%s::%s: : can't save to json because no saveToJson found", entityType.c_str(), componentType.c_str());
        return _componentsJson[entityType];
    }

    // Add entity in component entities map
    virtual void addComponent(const std::string& entityType, sComponent* component)
    {
        _components[entityType] = component;
    }

    // Save component json in case saveToJson is not overloaded and
    // we need to return the default json
    virtual void    saveComponentJson(const std::string& entityType, const JsonValue& json)
    {
        _componentsJson[entityType] = json;
    }

    // Overload this function to display the component editor
    // The component_ pointer have to be set to component pointer (_components[entityType] = component)
    virtual bool    updateEditor(const std::string& entityType, sComponent** component_)
    {
        return (false);
    }

private:
    virtual sComponent*         clone(const std::string& entityType)
    {
        return _components[entityType]->clone();
    }

protected:
    // One component per entity type
    std::unordered_map<std::string, sComponent*>    _components;

    // Store component JSON retrieved from loadFromJson function
    // so we can return the default json if saveToJson is not implemented
    std::unordered_map<std::string, JsonValue>    _componentsJson;
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
    virtual sComponent* loadFromJson(const std::string& entityType, const JsonValue& json);
    virtual JsonValue& saveToJson(const std::string& entityType, const std::string& componentType);

private:
    virtual bool    updateEditor(const std::string& entityType, sComponent** component_);
    virtual Sprite::eType stringToSpriteType(const std::string& spriteTypeStr);
    virtual std::string spriteTypeToString(Sprite::eType spriteType);
};


/*
** sPositionComponent
*/

template <>
class ComponentFactory<sPositionComponent>: public BaseComponentFactory<sPositionComponent>
{
public:
    virtual sComponent* loadFromJson(const std::string& entityType, const JsonValue& json);
    virtual JsonValue& saveToJson(const std::string& entityType, const std::string& componentType);
};


/*
** sInputComponent
*/

template <>
class ComponentFactory<sInputComponent>: public BaseComponentFactory<sInputComponent>
{
public:
    virtual sComponent* loadFromJson(const std::string& entityType, const JsonValue& json);
    virtual JsonValue& saveToJson(const std::string& entityType, const std::string& componentType);
};


/*
** sDirectionComponent
*/

template <>
class ComponentFactory<sDirectionComponent>: public BaseComponentFactory<sDirectionComponent>
{
public:
    virtual sComponent* loadFromJson(const std::string& entityType, const JsonValue& json);
    virtual JsonValue& saveToJson(const std::string& entityType, const std::string& componentType);
};


/*
** sRectHitboxComponent
*/

template <>
class ComponentFactory<sRectHitboxComponent>: public BaseComponentFactory<sRectHitboxComponent>
{
public:
    virtual sComponent* loadFromJson(const std::string& entityType, const JsonValue& json);
    virtual JsonValue& saveToJson(const std::string& entityType, const std::string& componentType);
};


/*
** sCircleHitboxComponent
*/

template <>
class ComponentFactory<sCircleHitboxComponent>: public BaseComponentFactory<sCircleHitboxComponent>
{
public:
    virtual sComponent* loadFromJson(const std::string& entityType, const JsonValue& json);
    virtual JsonValue& saveToJson(const std::string& entityType, const std::string& componentType);
};


/*
** sGravityComponent
*/

template <>
class ComponentFactory<sGravityComponent>: public BaseComponentFactory<sGravityComponent>
{
public:
    virtual sComponent* loadFromJson(const std::string& entityType, const JsonValue& json);
    virtual JsonValue& saveToJson(const std::string& entityType, const std::string& componentType);
};


/*
** sTypeComponent
*/

template <>
class ComponentFactory<sTypeComponent>: public BaseComponentFactory<sTypeComponent>
{
public:
    virtual sComponent* loadFromJson(const std::string& entityType, const JsonValue& json);
    virtual JsonValue& saveToJson(const std::string& entityType, const std::string& componentType);
    virtual eEntityType stringToEntityType(const std::string& entityTypeStr);
    virtual std::string entityTypeToString(eEntityType entityType);
};


/*
** sAIComponent
*/

template <>
class ComponentFactory<sAIComponent>: public BaseComponentFactory<sAIComponent>
{
public:
    virtual sComponent* loadFromJson(const std::string& entityType, const JsonValue& json);
};


/*
** sParticleEmitterComponent
*/

template <>
class ComponentFactory<sParticleEmitterComponent>: public BaseComponentFactory<sParticleEmitterComponent>
{
public:
    virtual sComponent* loadFromJson(const std::string& entityType, const JsonValue& json);
    virtual JsonValue& saveToJson(const std::string& entityType, const std::string& componentType);
    virtual bool    updateEditor(const std::string& entityType, sComponent** component_);
};


/*
**  sTowerAIComponent
*/

template <>
class ComponentFactory<sTowerAIComponent> : public BaseComponentFactory<sTowerAIComponent>
{
public:
    virtual sComponent* loadFromJson(const std::string& entityType, const JsonValue& json);
    virtual JsonValue&  saveToJson(const std::string& entityType, const std::string& componentType);

    virtual bool    updateEditor(const std::string& entityType, sComponent** component_);
};


/*
** sProjectileComponent
*/

template <>
class ComponentFactory<sProjectileComponent> : public BaseComponentFactory<sProjectileComponent>
{
public:
    virtual sComponent* loadFromJson(const std::string& entityType, const JsonValue& json);
    virtual JsonValue&  saveToJson(const std::string& entityType, const std::string& componentType);

    virtual bool    updateEditor(const std::string& entityType, sComponent** component_);
};