#include "Utils/Exception.hpp"
#include "Graphics/Animation.hpp"

Animation::Animation(): _framesNb(0), _currentFrame(0), _spriteSheet(nullptr) {}

Animation::~Animation() {}

void    Animation::setSpriteSheet(Texture* spriteSheet)
{
    _spriteSheet = spriteSheet;
}

void    Animation::addFrame(const glm::vec2& offset)
{
    unsigned int spriteSheetSize;

    if (!_spriteSheet)
    {
        EXCEPT(InternalErrorException, "Attempt to add a frame without sprite sheet");
    }

    spriteSheetSize = _spriteSheet->getWidth() *  _spriteSheet->getHeight();

    if (offset.x > spriteSheetSize ||
        offset.y > spriteSheetSize)
    {
        EXCEPT(InternalErrorException, "Attempt to add a frame outside the sprite sheet range");
    }

    // Normalize texture coordinates
    glm::vec2 normalizedOffset;
    normalizedOffset.x = offset.x / _spriteSheet->getWidth();
    normalizedOffset.y = offset.y / _spriteSheet->getHeight();

    // Add textures coordinates to frame list
    _frames.push_back(normalizedOffset);
    ++_framesNb;
}

void    Animation::play(GLint textureShiftUniform)
{
    // Bind sprite sheet
    _spriteSheet->bind();

    // Define texture coordinates shift
    glUniform2f(textureShiftUniform, _frames[_currentFrame].x, _frames[_currentFrame].y);

    _currentFrame += 1;
    if (_currentFrame >= _framesNb)
    {
        _currentFrame = 0;
    }
}

void    Animation::reset()
{
    _currentFrame = 0;
}
