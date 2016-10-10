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

public:
    Sprite(Sprite::eType type, const ShaderProgram& shaderProgram);
    ~Sprite();

    void                                            loadFromTexture(const std::string& textureFile, bool animated,  uint32_t nbFrames, const std::vector<eOrientation>& orientations, const glm::vec2& spriteSize);
    void                                            update(glm::vec2 position, float z, bool keyPressed, eOrientation orientation);
    void                                            draw();
    const glm::vec3&                                getPos() const;

private:
    // OpenGL Buffer
    Buffer                                          _buffer;

    // Sprite texture
    Texture*                                        _texture;
    std::unordered_map<eOrientation, Animation>     _animations;
    eOrientation                                    _currentOrientation;

    // Sprite position
    glm::vec3                                       _pos;
    // Sprite spriteSize
    glm::vec2                                       _spriteSize;

    // Sprite type
    eType                                           _type;

    bool                                            _animated;

    // Vertex shader textureShift uniform vec2
    GLint                                           _textureShiftUniform;
};
