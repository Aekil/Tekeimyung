/**
* @Author   Guillaume Labey
*/

#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>

#include <ECS/Entity.hpp>
#include <Engine/Debug/Logger.hpp>
#include <Engine/Utils/Exception.hpp>
#include <Engine/Utils/JsonReader.hpp>

#define COMPONENTS_TYPES(PROCESS)\
    PROCESS(sRenderComponent),\
    PROCESS(sBoxColliderComponent),\
    PROCESS(sSphereColliderComponent),\
    PROCESS(sRigidBodyComponent),\
    PROCESS(sParticleEmitterComponent),\
    PROCESS(sNameComponent),\
    PROCESS(sTransformComponent),\
    PROCESS(sButtonComponent),\
    PROCESS(sScriptComponent),\
    PROCESS(sUiComponent),\
    PROCESS(sTextComponent),\
    PROCESS(sLightComponent),\
    PROCESS(sCameraComponent),\
    PROCESS(sDynamicComponent),

#define GENERATE_PAIRS(COMPONENT) { #COMPONENT, new ComponentFactory<COMPONENT>() }

#define GENERATE_PAIRS_HASHS(COMPONENT) { COMPONENT::identifier, #COMPONENT }

class IComponentFactory
{
public:
    IComponentFactory() = default;
    virtual ~IComponentFactory() = default;

    // Json load/save
    virtual sComponent* loadFromJson(const std::string& entityType, const JsonValue& json) = 0;
    // The component to save can be pass to saveToJson, else it will save the entity template component in _components[entityType]
    // The destination json can also be pass to saveToJson, else it will save the entity template component json in _componentsJson[entityType]
    virtual JsonValue& saveToJson(const std::string& entityType, const sComponent* savedComponent = nullptr, JsonValue* toJson = nullptr) = 0;
    virtual void save(const std::string& entityType, sComponent* component) = 0;
    virtual void remove(const std::string& entityType) = 0;

    // IComponentFactory methods
    static bool                                                     componentTypeExists(const std::string& type);
    static void                                                     initComponent(const std::string& entityType, const std::string& name, const JsonValue& value);
    static sComponent*                                              createComponent(const std::string& entityType, const std::string& name);
    static IComponentFactory*                                       getFactory(const std::string& name);

    static std::string                                              getComponentNameWithHash(uint32_t hash);
    static std::size_t                                              getComponentHashWithName(const std::string& name);
    static const std::unordered_map<uint32_t, std::string>&         getComponentsTypesHashs();

    // ComponentFactory overloaded classes methods
    // Ex: ComponentFactory<sInputComponent>
    virtual sComponent*                                             clone(const std::string& entityType) = 0;
    virtual void                                                    addComponent(const std::string& entityType, sComponent* component) = 0;
    virtual void                                                    saveComponentJson(const std::string& entityType, const JsonValue& json) = 0;
    virtual bool                                                    updateEditor(const std::string& entityType, sComponent** savedComponent, sComponent* entityComponent, Entity* entity) = 0;

protected:
    // Store Components types
    static std::unordered_map<std::string, IComponentFactory*>      _componentsTypes;

    // Map lookup to get component name with component hash
    static std::unordered_map<uint32_t, std::string>             _componentsTypesHashs;
};


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
    virtual JsonValue& saveToJson(const std::string& entityType, const sComponent* savedComponent = nullptr, JsonValue* toJson = nullptr)
    {
        LOG_WARN("%s::%s: : can't save to json because no saveToJson found. The previously loaded json will be saved", entityType.c_str(), getTypeName());
        return _componentsJson[entityType];
    }

    void save(const std::string& entityType, sComponent* component)  override final
    {
        delete _components[entityType];
        _components[entityType] = component->clone();
    }

    void remove(const std::string& entityType)  override final
    {
        _components.erase(entityType);
    }

    // Add entity in component entities map
    void addComponent(const std::string& entityType, sComponent* component)  override final
    {
        _components[entityType] = component;
    }

    // Save component json in case saveToJson is not overloaded and
    // we need to return the default json
    void    saveComponentJson(const std::string& entityType, const JsonValue& json) override final
    {
        _componentsJson[entityType] = json;
    }

    // Overload this function to display the component editor
    // The component_ pointer have to be set to component pointer (_components[entityType] = component)
    virtual bool    updateEditor(const std::string& entityType, sComponent** savedComponent, sComponent* entityComponent, Entity* entity)
    {
        return (false);
    }

private:
    virtual const char* getTypeName() = 0;
    sComponent*         clone(const std::string& entityType)
    {
        return _components.at(entityType)->clone();
    }

protected:
    // One component per entity type
    std::unordered_map<std::string, sComponent*>    _components;

    // Store component JSON retrieved from loadFromJson function
    // so we can return the default json if saveToJson is not implemented
    std::unordered_map<std::string, JsonValue>    _componentsJson;
};


template <typename T>
class ComponentFactory
{
public:
    ComponentFactory() = default;
    ~ComponentFactory() = default;
};
