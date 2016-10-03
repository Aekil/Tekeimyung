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
    float spriteWidth;
    float spriteHeight;

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

        spriteWidth = spriteSize.x;
        spriteHeight = spriteSize.y;
    }
    else
    {
        spriteWidth = _texture->getWidth();
        spriteHeight = _texture->getHeight();
    }

    // Init entity buffers
    Vertex vertices[] = {
        {glm::vec3(0.0f,  spriteHeight, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},  // Top Left (red)
        {glm::vec3(spriteWidth,  spriteHeight, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(spriteWidth / _texture->getWidth(), 0.0f)},  // Top Right (blue)
        {glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, spriteHeight / _texture->getHeight())},  // Bottom Left (green)
        {glm::vec3(spriteWidth, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(spriteWidth / _texture->getWidth(), spriteHeight / _texture->getHeight())}  // Bottom Right (red)
    };

    GLuint indices[] = {
        0, 1, 2,
        1, 2, 3
    };

   _buffer.updateData(vertices, 4, indices, 6);
}

void Sprite::update(float x, float y, float z, bool keyPressed, eOrientation orientation)
{
    float offsetX = GameWindow::getInstance()->getWidth() / 2.0f - 66.0f;
    float offsetY = GameWindow::getInstance()->getHeight() - (33.0f * 3.0f);

    _pos.x = offsetX + (x - y) * 66.0f;
    _pos.y = offsetY - (x + y) * 33.0f + (32.0f * z);
    _pos.z = z;
    _currentOrientation = orientation;

    if (_type == Sprite::eType::OBJECT)
    {
        _pos.y += 30.0f;
    }
    if (_animated && !keyPressed)
    {
        _animations[_currentOrientation].reset();
    }
}

void    Sprite::draw()
{
    if (_animated)
    {
        // Play animation
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
