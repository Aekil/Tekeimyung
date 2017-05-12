#pragma once

#include <cstdint>
#include <string>

#include <ECS/Component.hh>
#include <Engine/Utils/Helper.hpp>

#define BUTTON_ACTION(PROCESS)              \
    PROCESS(NONE)                           \
    PROCESS(REMOVE_CURRENT_LEVEL)           \
    PROCESS(REPLACE_CURRENT_LEVEL)          \
    PROCESS(ADD_LEVEL)


START_COMPONENT(sButtonComponent)
REGISTER_ENUM(eAction, uint8_t, BUTTON_ACTION);

virtual sComponent* clone()
{
    sButtonComponent* component = new sButtonComponent();
    component->update(this);

    return (component);
}

virtual void update(sButtonComponent* component)
{
    this->selected = component->selected;
    this->hovered = component->hovered;
    this->action = component->action;
    this->actionLevel = component->actionLevel;
    this->removeStates = component->removeStates;
}

virtual void update(sComponent* component)
{
    update(static_cast<sButtonComponent*>(component));
}

bool        selected = false;
bool        hovered = false;
eAction     action = eAction::NONE;
std::string actionLevel;
// We can remove states before action
uint32_t    removeStates = 0;
END_COMPONENT(sButtonComponent)
REGISTER_ENUM_MANAGER(sButtonComponent::eAction, BUTTON_ACTION);
