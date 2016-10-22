#include <iostream>
#include "Window/GameWindow.hpp"
#include "Utils/RessourceManager.hpp"

#include "Graphics/Sprite.hpp"


Sprite::Sprite(eType type, const ShaderProgram& shaderProgram): _type(type), _texture(nullptr), _currentOrientation(eOrientation::N)
{
    _textureShiftUniform = shaderProgram.getUniformLocation("texCoordsShift");
}

Sprite::~Sprite() {}

void    Sprite::loadFromTexture(const std::string& textureFile, bool animated,  uint32_t nbFrames, const std::vector<eOrientation>& orientations, const glm::vec2& spriteSize)
{
    // Init entity texture
    _texture = &RessourceManager::getInstance()->getTexture(textureFile);
    _animated = animated;

    if (_animated)
    {
        for (unsigned int i = 0; i < orientations.size(); i++)
        {
            auto &&orientation = orientations[i];
            glm::vec2 offset(0, spriteSize.y * i / _texture->getHeight());

            _animations[orientation] = {};
            _animations[orientation].setSpriteSheet(_texture);
            for (unsigned int frame = 0; frame < nbFrames; frame++)
            {
                _animations[orientation].addFrame(offset);
                offset.x += spriteSize.x / _texture->getWidth();
            }
        }

        _spriteSize = spriteSize;
    }
    else
    {
        _spriteSize.x = _texture->getWidth();
        _spriteSize.y = _texture->getHeight();
    }

    // Init entity buffers
    Vertex vertices[] = {
        {glm::vec3(0.0f,  _spriteSize.y, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},  // Top Left (red)
        {glm::vec3(_spriteSize.x,  _spriteSize.y, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(_spriteSize.x / _texture->getWidth(), 0.0f)},  // Top Right (blue)
        {glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, _spriteSize.y / _texture->getHeight())},  // Bottom Left (green)
        {glm::vec3(_spriteSize.x, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(_spriteSize.x / _texture->getWidth(), _spriteSize.y / _texture->getHeight())}  // Bottom Right (red)
    };

    GLuint indices[] = {
        0, 1, 2,
        1, 2, 3
    };

   _buffer.updateData(vertices, 4, indices, 6);
}

void Sprite::update(glm::vec2 position, float z, bool keyPressed, eOrientation orientation)
{
    float tileWidthHalf = 64.0f;
    float tileLengthHalf = 32.0f;
    float tileHeight = 38.0f;
    float offsetX = GameWindow::getInstance()->getWidth() / 2.0f - tileWidthHalf;
    float offsetY = GameWindow::getInstance()->getHeight() - (tileLengthHalf * 3.0f);

    _pos.x = offsetX + (position.x - position.y) * tileWidthHalf;
    _pos.y = offsetY - (position.x + position.y) * tileLengthHalf + (tileHeight * z);
    _pos.z = z;
    _currentOrientation = orientation;

    if (_type == Sprite::eType::OBJECT)
    {
        _pos.y += _spriteSize.y;
        _pos.x += _spriteSize.x - 15.0f;
    }
    else
    {
        _pos.x -= (_spriteSize.x / 2.0f) - tileWidthHalf;
    }
    if (_animated && !keyPressed)
    {
        _animations[_currentOrientation].reset();
    }
}

void    Sprite::draw()
{
    if (_animated && _animations.find(_currentOrientation) != _animations.end())
    {
        // Play animation if found for current orientation
        _animations[_currentOrientation].play(_textureShiftUniform);
    }
    else
    {
        // Bind texture
        _texture->bind();
    }

    // Bind Vertex array buffer
    _buffer.bind();

    // Draw to screen
    glDrawElements(GL_TRIANGLES, _buffer.getIndicesNb(), GL_UNSIGNED_INT, 0);

    // Reset texture shift
    glUniform2f(_textureShiftUniform, 0, 0);
}

const glm::vec3&    Sprite::getPos() const
{
    return _pos;
}
