/**
* @Author   Guillaume Labey
*/

#include <cstring>

#include <Engine/Graphics/UniformBuffer.hpp>
#include <Engine/Utils/Debug.hpp>

UniformBuffer::UniformBuffer(): _UBO(0), _bindingPoint(0), _init(false), _size(0)
{
    glGenBuffers(1, &_UBO);
}


UniformBuffer::~UniformBuffer() {}

void    UniformBuffer::init(uint32_t size)
{
    ASSERT(_init == false, "An UniformBuffer should not be initialized 2 times");

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
    ASSERT(_init == true, "An UniformBuffer should be initialized before updating");

    // Bind UBO to GL_UNIFORM_BUFFER type so that all calls to GL_UNIFORM_BUFFER use VBO
    glBindBuffer(GL_UNIFORM_BUFFER, _UBO);

    // Update buffer data
    glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);

    // Unbind UBO
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void    UniformBuffer::bind(const ShaderProgram& shaderProgram, const char* blockName, uint32_t offset, uint32_t size)
{
    ASSERT(_init == true, "An UniformBuffer should be initialized before binding");

    if (blockName != _boundBlock)
    {
        _boundBlock = blockName;

        // Uniform block index in shader
        _boundBlockIndex = shaderProgram.getUniformBlockIndex(blockName);
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
