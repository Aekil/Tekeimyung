/**
* @Author   Guillaume Labey
*/

#pragma once

#include <glm/vec2.hpp>

#include <ECS/Component.hh>
#include <Engine/Graphics/BufferPool.hpp>
#include <Engine/Graphics/UI/Text.hpp>
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

START_COMPONENT(sTextComponent)
virtual sComponent* clone()
{
    sTextComponent* component = new sTextComponent();
    component->update(this);

    return (component);
}

virtual void update(sTextComponent* component)
{
    this->text = component->text;
    this->horizontalAlignment = component->horizontalAlignment;
    this->verticalAlignment = component->verticalAlignment;
    this->alignmentOffset = component->alignmentOffset;
    this->offset = component->offset;
}

virtual void update(sComponent* component)
{
    update(static_cast<sTextComponent*>(component));
}

BufferPool::SubBuffer*  getBuffer() const
{
    return (_buffer);
}

BufferPool::SubBuffer*  getBuffer(BufferPool* bufferPool)
{
    if (!_buffer)
    {
        _buffer = bufferPool->allocate();
    }
    return (_buffer);
}

Text text;
eHorizontalAlignment    horizontalAlignment = eHorizontalAlignment::MIDDLE;
eVerticalAlignment      verticalAlignment = eVerticalAlignment::MIDDLE;
glm::vec2 alignmentOffset; // Offset to align text
glm::vec2 offset; // Text offset

private:
BufferPool::SubBuffer* _buffer{nullptr};
END_COMPONENT(sTextComponent)
