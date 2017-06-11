/**
* @Author   Guillaume Labey
*/

#pragma once

#include <string>

#include <ECS/Component.hh>

START_COMPONENT(sNameComponent)
sNameComponent(const std::string& name) : value(name), sComponent(sNameComponent::identifier) {}

virtual sComponent* clone()
{
    sNameComponent* component = new sNameComponent();
    component->update(this);

    return (component);
}

virtual void update(sNameComponent* component)
{
    this->value = component->value;
}

virtual void update(sComponent* component)
{
    update(static_cast<sNameComponent*>(component));
}

std::string value;
END_COMPONENT(sNameComponent)
