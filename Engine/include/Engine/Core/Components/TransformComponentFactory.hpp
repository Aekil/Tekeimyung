/**
* @Author   Guillaume Labey
*/

#pragma once

#include <Engine/Core/Components/IComponentFactory.hpp>
#include <Engine/Core/Components/TransformComponent.hh>

template <>
class ComponentFactory<sTransformComponent> final: public BaseComponentFactory<sTransformComponent>
{
public:
    const char* getTypeName() override final { return "sTransformComponent"; }
    sComponent* loadFromJson(const std::string& entityType, const JsonValue& json) override final;
    JsonValue&  saveToJson(const std::string& entityType, const sComponent* savedComponent = nullptr, JsonValue* toJson = nullptr) override final;

    bool    updateEditor(const std::string& entityType, sComponent** savedComponent, sComponent* entityComponent, Entity* entity) override final;

    // Update transforms with gizmos
    // Used for multiple components: sTransformComponent, sBoxColliderComponent
    static bool     updateTransforms(glm::vec3& pos, glm::vec3& scale, glm::vec3& rotation, glm::mat4& transform);

    static bool enableGuizmos;
};
