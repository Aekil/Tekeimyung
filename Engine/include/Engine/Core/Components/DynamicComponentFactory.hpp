/**
* @Author   Guillaume Labey
*/

#pragma once

#include <Engine/Core/Components/IComponentFactory.hpp>
#include <Engine/Core/Components/DynamicComponent.hh>

template <>
class ComponentFactory<sDynamicComponent> final : public BaseComponentFactory<sDynamicComponent>
{
public:
    char* getTypeName() override final { return "sDynamicComponent"; }

    sComponent* loadFromJson(const std::string& entityType, const JsonValue& json) override final;
    JsonValue&  saveToJson(const std::string& entityType, const sComponent* savedComponent = nullptr, JsonValue* toJson = nullptr) override final;
};
