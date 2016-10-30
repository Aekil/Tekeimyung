#pragma once

#include <string>
#include <cstdint>
#include <vector>
#include <glm/vec2.hpp>
#include "Graphics/Texture.hpp"

enum class eOrientation
{
    N,
    NE,
    E,
    SE,
    S,
    SW,
    W,
    NW
};

class Animation
{
public:
    Animation();
    ~Animation();
    void                                    setSpriteSheet(Texture* spriteSheet);
    void                                    addFrame(const glm::vec2& offset);
    void                                    addFrames(const glm::vec2& baseOffset, const glm::vec2& spriteSize, uint32_t cols, uint32_t rows);
    void                                    play(GLint textureShiftUniform);
    void                                    reset();

private:
    // Texture uv coordinates offset
    std::vector<glm::vec2>                  _frames;

    uint32_t                                _framesNb;
    uint32_t                                _currentFrame;

    Texture*                                _spriteSheet;
};
