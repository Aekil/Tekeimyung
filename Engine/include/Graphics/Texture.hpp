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
    int                                     getBitsPerComponent() const;

private:
    GLuint                                  _texture;
    int                                     _width;
    int                                     _height;
    unsigned char*                          _data;

    // Bits per component
    int                                     _bpc;
};
