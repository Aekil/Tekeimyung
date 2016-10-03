#define STB_IMAGE_IMPLEMENTATION

#include "Utils/EngineException.hpp"
#include "stb_image/stb_image.h"

#include "Graphics/Texture.hpp"

Texture::Texture(): _data(nullptr), _bpc(0) {}

Texture::~Texture()
{
    if (_data)
    {
        // Free image data
        STBI_FREE(_data);
    }
}

void    Texture::loadFromFile (const std::string &fileName)
{
    _data = stbi_load(fileName.c_str(), &_width, &_height, &_bpc, 0);

    if (_data == nullptr)
    {
         throw EngineException("Failed to load ", fileName, ": ", stbi_failure_reason());
    }
    else if (_bpc != 4)
    {
        throw EngineException("NotImplemented: can't read textures with less than 4 bits components");
    }

    // Create one texture object
    glGenTextures(1, &_texture);

    // Use texture
    glBindTexture(GL_TEXTURE_2D, _texture);

    // Copy image data into texture object
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, _data);
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

unsigned char*  Texture::getData() const
{
    return _data;
}

int     Texture::getBitsPerComponent() const
{
    return _bpc;
}
