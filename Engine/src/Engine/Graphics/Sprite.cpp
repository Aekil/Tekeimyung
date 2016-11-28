#include <iostream>

#include <Engine/Window/GameWindow.hpp>
#include <Engine/Utils/RessourceManager.hpp>
#include <Engine/Utils/Logger.hpp>

#include <Engine/Graphics/Sprite.hpp>


Sprite::Sprite(eType type, const ShaderProgram& shaderProgram): _type(type), _texture(nullptr), _currentOrientation(eOrientation::N), _color({0.0f, 0.0f, 0.0f})
{
    _textureShiftUniform = shaderProgram.getUniformLocation("texCoordsShift");
}

Sprite::~Sprite() {}

void    Sprite::loadFromTexture(sCreateInfo& createInfo)
{
    // Init entity texture
    _texture = &RessourceManager::getInstance()->getTexture(createInfo.textureFile);
    _animated = createInfo.animated;
    _color = createInfo.color;

    if (_animated)
    {
        _spriteSize = createInfo.spriteSize;

        // Load sprites with given orientations
        if (createInfo.orientations.size() > 0)
        {
            for (unsigned int i = 0; i < createInfo.orientations.size(); i++)
            {
                auto &&orientation = createInfo.orientations[i];

                _animations[orientation] = {};
                _animations[orientation].setSpriteSheet(_texture);
                _animations[orientation].addFrames({createInfo.offset.x, createInfo.offset.y + i * _spriteSize.y}, _spriteSize, createInfo.frames.x, 1);
            }
        }
        // Load sprites with default orientation
        else
        {
            _animations[_currentOrientation] = {};
            _animations[_currentOrientation].setSpriteSheet(_texture);
            _animations[_currentOrientation].addFrames({createInfo.offset.x, createInfo.offset.y}, _spriteSize, createInfo.frames.x, createInfo.frames.y);
        }
    }
    else
    {
        _spriteSize = {_texture->getWidth(), _texture->getHeight()};
    }

    // Init entity buffers
/*    Vertex vertices[] = {
        {glm::vec3(0.0f,  _spriteSize.y, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},  // Top Left
        {glm::vec3(_spriteSize.x,  _spriteSize.y, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(_spriteSize.x / _texture->getWidth(), 0.0f)},  // Top Right
        {glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, _spriteSize.y / _texture->getHeight())},  // Bottom Left
        {glm::vec3(_spriteSize.x, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(_spriteSize.x / _texture->getWidth(), _spriteSize.y / _texture->getHeight())}  // Bottom Right
    };

    GLuint indices[] = {
        0, 1, 2,
        1, 2, 3
    };
*/
/*   _buffer.updateData(vertices, 4, indices, 6);*/
}

void Sprite::update(glm::vec3& position, bool moved, eOrientation orientation, glm::vec3& color)
{
    _pos = position;
    _currentOrientation = orientation;
    _color = color;

    if (_animated && !moved && _animations.size() > 1)
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
    return (_pos);
}

const glm::vec3&    Sprite::getColor() const
{
    return (_color);
}

const Sprite::eType&    Sprite::getType() const
{
    return (_type);
}

const glm::vec2&    Sprite::getSpriteSize() const
{
    return (_spriteSize);
}