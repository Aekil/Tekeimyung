/**
* @Author   Guillaume Labey
*/

#include <cstring>

#include <Engine/Graphics/UniformBuffer.hpp>
#include <Engine/Utils/Logger.hpp>

UniformBuffer::UniformBuffer(): _UBO(0), _bindingPoint(0), _init(false), _size(0)
{
    glGenBuffers(1, &_UBO);
}


UniformBuffer::~UniformBuffer() {}

void    UniformBuffer::init(uint32_t size)
{
    if (_init)
        return;

    // Bind UBO to GL_UNIFORM_BUFFER type so that all calls to GL_UNIFORM_BUFFER use VBO
    glBindBuffer(GL_UNIFORM_BUFFER, _UBO);

    // Update buffer data
    glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);

    // Unbind UBO
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    _size = size;
    _init = true;
}

void    UniformBuffer::update(void* data, uint32_t size, uint32_t offset)
{
    if (!_init)
    {
        LOG_WARN("UniformBuffer::update: Can't update buffer which is not initialized");
        return;
    }

    // Bind UBO to GL_UNIFORM_BUFFER type so that all calls to GL_UNIFORM_BUFFER use VBO
    glBindBuffer(GL_UNIFORM_BUFFER, _UBO);

    // Update buffer data
    glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);

    // Unbind UBO
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void    UniformBuffer::bind(uint32_t offset, uint32_t size)
{
    if (!_init)
    {
        LOG_WARN("UniformBuffer::bind: Can't bind buffer which is not initialized");
        return;
    }

    if (!size)
        size = _size;

    // Bind UBO
    glBindBufferRange(GL_UNIFORM_BUFFER, _bindingPoint, _UBO, offset, size);
}

void    UniformBuffer::setBindingPoint(uint16_t bindingPoint)
{
    _bindingPoint = bindingPoint;
}
