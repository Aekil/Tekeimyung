/**
* @Author   Guillaume Labey
*/

#pragma once

#include <Engine/Core/Components/IComponentFactory.hpp>
#include <Engine/Core/Components/UiComponent.hh>

template <>
class ComponentFactory<sUiComponent> final: public BaseComponentFactory<sUiComponent>
{
public:
    char* getTypeName() override final { return "sUiComponent"; }
    sComponent* loadFromJson(const std::string& entityType, const JsonValue& json) override final;
    JsonValue&  saveToJson(const std::string& entityType, const sComponent* savedComponent = nullptr, JsonValue* toJson = nullptr) override final;

    bool    updateEditor(const std::string& entityType, sComponent** savedComponent, sComponent* entityComponent, Entity* entity) override final;
};
