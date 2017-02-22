/**
* @Author   Guillaume Labey
*/

#include <cstring>

#include <Engine/Graphics/UniformBuffer.hpp>
#include <Engine/Utils/Logger.hpp>

UniformBuffer::UniformBuffer(): _UBO(0), _bindingPoint(0), _init(false), _size(0)
{
    _bufferType = GL_UNIFORM_BUFFER;
    glGenBuffers(1, &_UBO);
}


UniformBuffer::~UniformBuffer()
{
    glDeleteBuffers(1, &_UBO);
}

void    UniformBuffer::init(uint32_t size, GLuint bufferType)
{
    if (_init)
        return;

    _bufferType = bufferType;

    // Bind UBO to _bufferType type so that all calls to _bufferType use VBO
    glBindBuffer(_bufferType, _UBO);

    // Update buffer data
    glBufferData(_bufferType, size, nullptr, GL_DYNAMIC_DRAW);

    // Unbind UBO
    glBindBuffer(_bufferType, 0);

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

    // Bind UBO to _bufferType type so that all calls to _bufferType use VBO
    glBindBuffer(_bufferType, _UBO);

    // Update buffer data
    glBufferSubData(_bufferType, offset, size, data);

    // Unbind UBO
    glBindBuffer(_bufferType, 0);
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
    glBindBufferRange(_bufferType, _bindingPoint, _UBO, offset, size);
}

void    UniformBuffer::setBindingPoint(uint16_t bindingPoint)
{
    _bindingPoint = bindingPoint;
}
