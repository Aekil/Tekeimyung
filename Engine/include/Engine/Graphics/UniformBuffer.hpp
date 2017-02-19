/**
* @Author   Guillaume Labey
*/

#pragma once

#include <GL/glew.h>
#include <cstdint>

#include <Engine/Graphics/ShaderProgram.hpp>

class UniformBuffer
{
public:
    UniformBuffer();
    ~UniformBuffer();

    void                    init(uint32_t size);
    void                    update(void* data, uint32_t size, uint32_t offset = 0);

    // Bind UBO with uniform buffer block in shader
    void                    bind(const ShaderProgram& shaderProgram, const char* blockName, uint32_t offset = 0, uint32_t size = 0);

    void                    setBindingPoint(uint16_t bindingPoint);

private:
    // Uniform buffer object
    GLuint                  _UBO;

    GLuint                  _bindingPoint;

    std::string             _boundBlock;
    GLuint                  _boundBlockIndex;

    bool                    _init;
    uint32_t                _size;
};
