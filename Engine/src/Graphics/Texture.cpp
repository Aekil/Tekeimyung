#define STB_IMAGE_IMPLEMENTATION

#include "Utils/EngineException.hpp"
#include "stb_image/stb_image.h"

#include "Graphics/Texture.hpp"

void    Texture::loadFromFile (const std::string &fileName)
{
    unsigned char* imageData = stbi_load(fileName.c_str(), &_width, &_height, &_bpc, 0);

    if (imageData == nullptr)
    {
         throw EngineException("Failed to load ", fileName, ": ", stbi_failure_reason());
    }
/*    sf::Image image;

    if (!image.loadFromFile(fileName)) {
        throw EngineException("Failed to load ", fileName);
    }
    _width = image.getSize().x;
    _height = image.getSize().y;*/

    // Create one texture object
    glGenTextures(1, &_texture);

    // Use texture
    glBindTexture(GL_TEXTURE_2D, _texture);

    // Copy image data into texture object
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
    // Generate image mipmap levels
    glGenerateMipmap(GL_TEXTURE_2D);

    // Unuse texture
    glBindTexture(GL_TEXTURE_2D, 0);
}

void    Texture::bind() const
{
    glBindTexture(GL_TEXTURE_2D, _texture);
}

unsigned int    Texture::getWidth() const
{
    return _width;
}

unsigned int    Texture::getHeight() const
{
    return _height;
}