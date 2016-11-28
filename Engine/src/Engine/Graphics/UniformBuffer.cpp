#include <cstring>

#include <Engine/Graphics/UniformBuffer.hpp>

UniformBuffer::UniformBuffer(): _UBO(0), _firstUpdate(true), _bindingPoint(0)
{
    glGenBuffers(1, &_UBO);
}


UniformBuffer::~UniformBuffer() {}

void    UniformBuffer::update(void* data, uint32_t size)
{
    if (_firstUpdate)
    {
        // Bind UBO to GL_UNIFORM_BUFFER type so that all calls to GL_UNIFORM_BUFFER use VBO
        glBindBuffer(GL_UNIFORM_BUFFER, _UBO);

        // Update buffer data
        glBufferData(GL_UNIFORM_BUFFER, size, data, GL_DYNAMIC_COPY);

        // Unbind UBO
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        _firstUpdate = false;
    }
    else
    {
        // Bind UBO to GL_UNIFORM_BUFFER type so that all calls to GL_UNIFORM_BUFFER use VBO
        glBindBuffer(GL_UNIFORM_BUFFER, _UBO);

        // Get GPU pointer and update data
        GLvoid* gpuPtr = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
        memcpy(gpuPtr, data, size);
        glUnmapBuffer(GL_UNIFORM_BUFFER);

        // Unbind UBO
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }
}

void    UniformBuffer::bind(const ShaderProgram& shaderProgram, const char* blockName) const
{
    // Uniform block index in shader
    GLuint uniformBlockIndex = shaderProgram.getUniformBlockIndex(blockName);

    // Bind UBO
    glBindBufferBase(GL_UNIFORM_BUFFER, _bindingPoint, _UBO);

    // Bind UBO with uniform block in shader
    shaderProgram.bindBlock(uniformBlockIndex, _bindingPoint);
}

void    UniformBuffer::setBindingPoint(uint16_t bindingPoint)
{
    _bindingPoint = bindingPoint;
}
