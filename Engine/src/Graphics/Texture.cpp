#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include "Utils/Exception.hpp"
#include "stb_image/stb_image.h"

#include "Graphics/Texture.hpp"

Texture::Texture(): _data(nullptr), _comp(0) {}

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
    // Load image data and force  components number
    _data = stbi_load(fileName.c_str(), &_width, &_height, &_comp, 4);

    if (_data == nullptr)
    {
         EXCEPT(FileNotFoundException, "Failed to load file");
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

int     Texture::getComponentsNumber() const
{
    return _comp;
}
