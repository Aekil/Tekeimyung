/**
* @Author   Guillaume Labey
*/

#pragma once

#include <ImGuizmo.h>
#include <utility>
#include <algorithm>

#include <ECS/Entity.hpp>

#include <Engine/Utils/Exception.hpp>
#include <Engine/Utils/Logger.hpp>
#include <Engine/Utils/Debug.hpp>
#include <Engine/Components.hh>
#include <Engine/Utils/JsonValue.hpp>

// Generate map initializer lists
// ex: { "sRenderComponent", sRenderComponent }
#define COMPONENTS_TYPES(PROCESS)\
    PROCESS(sRenderComponent),\
    PROCESS(sBoxColliderComponent),\
    PROCESS(sSphereColliderComponent),\
    PROCESS(sRigidBodyComponent),\
    PROCESS(sTypeComponent),\
    PROCESS(sParticleEmitterComponent),\
    PROCESS(sNameComponent),\
    PROCESS(sTransformComponent),\
    PROCESS(sButtonComponent),\
    PROCESS(sScriptComponent),\
    PROCESS(sUiComponent)

#define GENERATE_PAIRS(COMPONENT) { #COMPONENT, new ComponentFactory<COMPONENT>() }

#define GENERATE_PAIRS_HASHS(COMPONENT) { COMPONENT::identifier, #COMPONENT }


class IParamAnimation;

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

    template<typename T>
    bool updateComboEnum(const char* name, T& enum_)
    {
        int enumInt = (int)enum_;

        if (ImGui::Combo(name, &enumInt, (const char**)EnumManager<T>::enumStrings, (int)EnumManager<T>::enumLength))
        {
            enum_ = (T)enumInt;
            return (true);
        }
        return (false);
    }

    bool updateComboString(const char* name, std::vector<const char*>& stringList, std::string& stringValue);

private:
    // Store Components types
    static std::unordered_map<std::string, IComponentFactory*>      _componentsTypes;

    // Map lookup to get component name with component hash
    static std::unordered_map<uint32_t, std::string>             _componentsTypesHashs;
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
    virtual char* getTypeName() = 0;
    sComponent*         clone(const std::string& entityType)
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
class ComponentFactory<sRenderComponent> final: public BaseComponentFactory<sRenderComponent>
{
public:
    char* getTypeName() override final { return "sRenderComponent"; }
    sComponent* loadFromJson(const std::string& entityType, const JsonValue& json) override final;
    JsonValue& saveToJson(const std::string& entityType, const sComponent* savedComponent = nullptr, JsonValue* toJson = nullptr) override final;

private:
    void            loadTranslateParamAnimation(std::shared_ptr<ParamAnimation<glm::vec3>> paramAnimation, JsonValue& json);
    void            loadColorParamAnimation(std::shared_ptr<ParamAnimation<glm::vec4>> paramAnimation, JsonValue& json);

    void            saveTranslateParamAnimation(std::shared_ptr<IParamAnimation> paramAnimation_, JsonValue& json);
    void            saveColorParamAnimation(std::shared_ptr<IParamAnimation> paramAnimation_, JsonValue& json);

    bool            updateEditor(const std::string& entityType, sComponent** savedComponent, sComponent* entityComponent, Entity* entity) override final;
    bool            updateAnimationsEditor(sRenderComponent* component, Entity* entity);
    bool            updateParamsAnimationsEditor(AnimationPtr playedAnimation, Entity* entity);
    bool            updateAnimationParamTranslate(Entity* entity, std::shared_ptr<IParamAnimation> paramAnimation_, uint32_t& frameNb);
    bool            updateAnimationParamColor(std::shared_ptr<IParamAnimation> paramAnimation_, uint32_t& frameNb);
};

/*
** sRigidBodyComponent
*/

template <>
class ComponentFactory<sRigidBodyComponent> final: public BaseComponentFactory<sRigidBodyComponent>
{
public:
    char* getTypeName() override final { return "sRigidBodyComponent"; }
    sComponent* loadFromJson(const std::string& entityType, const JsonValue& json) override final;
    JsonValue& saveToJson(const std::string& entityType, const sComponent* savedComponent = nullptr, JsonValue* toJson = nullptr) override final;
};


/*
** sBoxColliderComponent
*/

template <>
class ComponentFactory<sBoxColliderComponent> final: public BaseComponentFactory<sBoxColliderComponent>
{
public:
    char* getTypeName() override final { return "sBoxColliderComponent"; }
    sComponent* loadFromJson(const std::string& entityType, const JsonValue& json) override final;
    JsonValue& saveToJson(const std::string& entityType, const sComponent* savedComponent = nullptr, JsonValue* toJson = nullptr) override final;

    bool    updateEditor(const std::string& entityType, sComponent** savedComponent, sComponent* entityComponent, Entity* entity) override final;
};


/*
** sSphereColliderComponent
*/

template <>
class ComponentFactory<sSphereColliderComponent> final: public BaseComponentFactory<sSphereColliderComponent>
{
public:
    char* getTypeName() override final { return "sSphereColliderComponent"; }
    sComponent* loadFromJson(const std::string& entityType, const JsonValue& json) override final;
    JsonValue& saveToJson(const std::string& entityType, const sComponent* savedComponent = nullptr, JsonValue* toJson = nullptr) override final;

    bool    updateEditor(const std::string& entityType, sComponent** savedComponent, sComponent* entityComponent, Entity* entity) override final;
};


/*
** sTypeComponent
*/

template <>
class ComponentFactory<sTypeComponent> final: public BaseComponentFactory<sTypeComponent>
{
public:
    char* getTypeName() override final { return "sTypeComponent"; }
    sComponent* loadFromJson(const std::string& entityType, const JsonValue& json) override final;
    JsonValue& saveToJson(const std::string& entityType, const sComponent* savedComponent = nullptr, JsonValue* toJson = nullptr) override final;

    eEntityType stringToEntityType(const std::string& entityTypeStr);
    std::string entityTypeToString(eEntityType entityType);
};

/*
** sParticleEmitterComponent
*/

template <>
class ComponentFactory<sParticleEmitterComponent> final: public BaseComponentFactory<sParticleEmitterComponent>
{
public:
    char* getTypeName() override final { return "sParticleEmitterComponent"; }
    sComponent* loadFromJson(const std::string& entityType, const JsonValue& json) override final;
    JsonValue& saveToJson(const std::string& entityType, const sComponent* savedComponent = nullptr, JsonValue* toJson = nullptr) override final;

    bool    updateEditor(const std::string& entityType, sComponent** savedComponent, sComponent* entityComponent, Entity* entity) override final;
};

/*
** sNameComponent
*/

template <>
class ComponentFactory<sNameComponent> final: public BaseComponentFactory<sNameComponent>
{
public:
    char* getTypeName() override final { return "sNameComponent"; }
    sComponent* loadFromJson(const std::string& entityType, const JsonValue& json) override final;
    JsonValue& saveToJson(const std::string& entityType, const sComponent* savedComponent = nullptr, JsonValue* toJson = nullptr) override final;
};


/*
** sTransformComponent
*/

template <>
class ComponentFactory<sTransformComponent> final: public BaseComponentFactory<sTransformComponent>
{
public:
    char* getTypeName() override final { return "sTransformComponent"; }
    sComponent* loadFromJson(const std::string& entityType, const JsonValue& json) override final;
    JsonValue&  saveToJson(const std::string& entityType, const sComponent* savedComponent = nullptr, JsonValue* toJson = nullptr) override final;

    bool    updateEditor(const std::string& entityType, sComponent** savedComponent, sComponent* entityComponent, Entity* entity) override final;

    // Update transforms with gizmos
    // Used for multiple components: sTransformComponent, sBoxColliderComponent
    static bool     updateTransforms(glm::vec3& pos, glm::vec3& scale, glm::vec3& rotation, glm::vec3& savedRotation, glm::mat4& transform, ImGuizmo::MODE mode);
};


/*
** sButtonComponent
*/

template <>
class ComponentFactory<sButtonComponent> final: public BaseComponentFactory<sButtonComponent>
{
public:
    char* getTypeName() override final { return "sButtonComponent"; }
    sComponent* loadFromJson(const std::string& entityType, const JsonValue& json) override final;
    JsonValue&  saveToJson(const std::string& entityType, const sComponent* savedComponent = nullptr, JsonValue* toJson = nullptr) override final;

    bool    updateEditor(const std::string& entityType, sComponent** savedComponent, sComponent* entityComponent, Entity* entity) override final;
};

/*
** sScriptComponent
*/

template <>
class ComponentFactory<sScriptComponent> final: public BaseComponentFactory<sScriptComponent>
{
public:
    char* getTypeName() override final { return "sScriptComponent"; }
    sComponent* loadFromJson(const std::string& entityType, const JsonValue& json) override final;
    JsonValue&  saveToJson(const std::string& entityType, const sComponent* savedComponent = nullptr, JsonValue* toJson = nullptr) override final;

    bool    updateEditor(const std::string& entityType, sComponent** savedComponent, sComponent* entityComponent, Entity* entity) override final;
};


/*
** sUiComponent
*/

template <>
class ComponentFactory<sUiComponent> final: public BaseComponentFactory<sUiComponent>
{
public:
    char* getTypeName() override final { return "sUiComponent"; }
    sComponent* loadFromJson(const std::string& entityType, const JsonValue& json) override final;
    JsonValue&  saveToJson(const std::string& entityType, const sComponent* savedComponent = nullptr, JsonValue* toJson = nullptr) override final;

    bool    updateEditor(const std::string& entityType, sComponent** savedComponent, sComponent* entityComponent, Entity* entity) override final;
};
