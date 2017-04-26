/**
* @Author   Guillaume Labey
*/

#pragma once

#include <GL/glew.h>
#include <cstdint>

class UniformBuffer
{
public:
    UniformBuffer();
    ~UniformBuffer();

    void                    init(uint32_t size, GLuint bufferType = GL_UNIFORM_BUFFER);
    void                    update(void* data, uint32_t size, uint32_t offset = 0);

    // Bind UBO with uniform buffer block in shader
    void                    bind(uint32_t offset = 0, uint32_t size = 0);

    void                    setBindingPoint(uint16_t bindingPoint);

    bool                    isInit() const;
    uint32_t                getSize() const;

private:
    // Uniform buffer object
    GLuint                  _UBO;

    GLuint                  _bindingPoint;

    bool                    _init;
    uint32_t                _size;

    GLuint                  _bufferType;
};
