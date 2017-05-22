#pragma once

#include <Engine/Core/Components/IComponentFactory.hpp>
#include <Engine/Core/Components/RenderComponent.hh>

template <>
class ComponentFactory<sRenderComponent> final: public BaseComponentFactory<sRenderComponent>
{
public:
    char* getTypeName() override final { return "sRenderComponent"; }
    sComponent* loadFromJson(const std::string& entityType, const JsonValue& json) override final;
    JsonValue& saveToJson(const std::string& entityType, const sComponent* savedComponent = nullptr, JsonValue* toJson = nullptr) override final;

    static void     loadModelMaterialsFromJson(const std::string& entityType, ModelInstance* modelInstance, const JsonValue& json);
    static void     saveModelMaterialsFromJson(ModelInstance* modelInstance, JsonValue& json);
    static void     updateMaterialsEditor(ModelInstance* modelInstance);

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

private:
    AnimationPtr _lastAnimation = nullptr;
};
