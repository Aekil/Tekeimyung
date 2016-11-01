#pragma once

#include <GL/glew.h>
#include <unordered_map>
#include <string>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

#include "Graphics/Texture.hpp"
#include "Graphics/Animation.hpp"
#include "Graphics/Buffer.hpp"
#include "Graphics/ShaderProgram.hpp"

class Sprite
{
public:
    enum class eType
    {
        TILE,
        OBJECT
    };
    struct sCreateInfo
    {
        std::string textureFile;
        bool animated;
        glm::uvec2 frames;
        glm::vec2 offset;
        std::vector<eOrientation> orientations;
        glm::vec2 spriteSize;
        glm::vec3 color;
    };


public:
    Sprite(Sprite::eType type, const ShaderProgram& shaderProgram);
    ~Sprite();

    void                                            loadFromTexture(sCreateInfo& createInfo);
    void                                            update(glm::vec2 position, float z, bool moved, eOrientation orientation, glm::vec3& color);
    void                                            draw();
    const glm::vec3&                                getPos() const;
    const glm::vec3&                                getColor() const;

private:
    // OpenGL Buffer
    Buffer                                          _buffer;

    // Sprite texture
    Texture*                                        _texture;
    std::unordered_map<eOrientation, Animation>     _animations;
    eOrientation                                    _currentOrientation;

    // Sprite position
    glm::vec3                                       _pos;

    // Sprite color
    glm::vec3                                       _color;

    // Sprite spriteSize
    glm::vec2                                       _spriteSize;

    // Sprite type
    eType                                           _type;

    bool                                            _animated;

    // Vertex shader textureShift uniform vec2
    GLint                                           _textureShiftUniform;
};
