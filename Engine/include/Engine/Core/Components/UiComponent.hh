#pragma once

#include <glm/vec2.hpp>

#include <ECS/Component.hh>
#include <Engine/Utils/Helper.hpp>

#if !defined(HORIZONTAL_ALIGNMENT)
#define HORIZONTAL_ALIGNMENT(PROCESS)       \
    PROCESS(NONE)                           \
    PROCESS(LEFT)                           \
    PROCESS(MIDDLE)                         \
    PROCESS(RIGHT)

REGISTER_ENUM(eHorizontalAlignment, uint8_t, HORIZONTAL_ALIGNMENT);
REGISTER_ENUM_MANAGER(eHorizontalAlignment, HORIZONTAL_ALIGNMENT);
#endif

#if !defined(VERTICAL_ALIGNMENT)
#define VERTICAL_ALIGNMENT(PROCESS)         \
    PROCESS(NONE)                           \
    PROCESS(TOP)                            \
    PROCESS(MIDDLE)                         \
    PROCESS(BOTTOM)

REGISTER_ENUM(eVerticalAlignment, uint8_t, VERTICAL_ALIGNMENT);
REGISTER_ENUM_MANAGER(eVerticalAlignment, VERTICAL_ALIGNMENT);
#endif

START_COMPONENT(sUiComponent)
virtual sComponent* clone()
{
    sUiComponent* component = new sUiComponent();
    component->update(this);

    return (component);
}

virtual void update(sUiComponent* component)
{
    this->horizontalAlignment = component->horizontalAlignment;
    this->verticalAlignment = component->verticalAlignment;
    this->offset = component->offset;
    this->percentageSize = component->percentageSize;
    this->size = component->size;
    this->layer = component->layer;
    this->needUpdate = component->needUpdate;
}

virtual void update(sComponent* component)
{
    update(static_cast<sUiComponent*>(component));
}

eHorizontalAlignment    horizontalAlignment = eHorizontalAlignment::MIDDLE;
eVerticalAlignment      verticalAlignment = eVerticalAlignment::MIDDLE;
glm::vec2               offset{0.0f, 0.0f}; // Percentage offset
bool                    percentageSize{false}; // Use percentage for ui size
glm::vec2               size{0.1f, 0.1f}; // Percentage size
int                     layer{0}; // Layer used for UI ordering
bool                    needUpdate = true;

END_COMPONENT(sUiComponent)
