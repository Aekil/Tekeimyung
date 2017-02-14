/**
* @Author   Guillaume Labey
*/

#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include <stb_image/stb_image.h>

#include <Engine/Utils/Exception.hpp>
#include <Engine/Utils/Logger.hpp>

#include <Engine/Graphics/Texture.hpp>

Texture::Texture(): _data(nullptr), _comp(0), _unit(GL_TEXTURE0) {}

Texture::~Texture()
{
    if (_data)
    {
        // Free image data
        STBI_FREE(_data);
    }
}

bool    Texture::loadFromFile (const std::string &fileName)
{
    LOG_INFO("Loading texture \"%s\"", fileName.c_str());

    // Load image data and force  components number
    _data = stbi_load(fileName.c_str(), &_width, &_height, &_comp, 4);

    if (_data == nullptr)
    {
         EXCEPT(FileNotFoundException, "Failed to load texture \"%s\"", fileName.c_str());
    }

    // Create one texture object
    glGenTextures(1, &_texture);

    // Use texture
    glBindTexture(GL_TEXTURE_2D, _texture);

    // Define texture filter
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // Copy image data into texture object
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, _data);
    // Generate image mipmap levels
    glGenerateMipmap(GL_TEXTURE_2D);

    // Unuse texture
    glBindTexture(GL_TEXTURE_2D, 0);

    return (true);
}

void    Texture::setUnit(GLenum unit)
{
    _unit = unit;
}

void    Texture::bind() const
{
    glActiveTexture(_unit);
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
