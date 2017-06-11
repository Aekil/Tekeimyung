/**
* @Author   Guillaume Labey
*/

#pragma once

#include <Engine/Core/Components/IComponentFactory.hpp>
#include <Engine/Core/Components/RigidBodyComponent.hh>

template <>
class ComponentFactory<sRigidBodyComponent> final: public BaseComponentFactory<sRigidBodyComponent>
{
public:
    const char* getTypeName() override final { return "sRigidBodyComponent"; }
    sComponent* loadFromJson(const std::string& entityType, const JsonValue& json) override final;
    JsonValue& saveToJson(const std::string& entityType, const sComponent* savedComponent = nullptr, JsonValue* toJson = nullptr) override final;

    bool    updateEditor(const std::string& entityType, sComponent** savedComponent, sComponent* entityComponent, Entity* entity) override final;
};
