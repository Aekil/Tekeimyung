/**
* @Author   Guillaume Labey
*/

#pragma once

#include <Engine/Core/Components/IComponentFactory.hpp>
#include <Engine/Core/Components/NameComponent.hh>

template <>
class ComponentFactory<sNameComponent> final: public BaseComponentFactory<sNameComponent>
{
public:
    const char* getTypeName() override final { return "sNameComponent"; }
    sComponent* loadFromJson(const std::string& entityType, const JsonValue& json) override final;
    JsonValue& saveToJson(const std::string& entityType, const sComponent* savedComponent = nullptr, JsonValue* toJson = nullptr) override final;
};
