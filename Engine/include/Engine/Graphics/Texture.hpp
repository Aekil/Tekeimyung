#pragma once

#include <GL/glew.h>
#include <string>

class Texture
{
public:
    Texture();
    ~Texture();
    void                                    loadFromFile (const std::string& fileName);
    void                                    bind() const;
    unsigned int                            getWidth() const;
    unsigned int                            getHeight() const;
    unsigned char*                          getData() const;
    int                                     getComponentsNumber() const;

private:
    GLuint                                  _texture;
    int                                     _width;
    int                                     _height;
    unsigned char*                          _data;

    // Components number
    int                                     _comp;
};
