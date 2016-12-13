/**
* @Author   Guillaume Labey
*/

#pragma once

#include <GL/glew.h>
#include <string>

#include <Engine/Utils/Resource.hpp>

class Texture: public Resource
{
public:
    enum class eType: uint16_t
    {
        NONE = 0,
        DIFFUSE = 1,
        AMBIENT = 2
    };

public:
    Texture();
    ~Texture();
    void                                    loadFromFile (const std::string& fileName);

    void                                    setUnit(GLenum unit);
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

    GLenum                                  _unit;
};


inline int operator|(int& lhs, const Texture::eType& rhs) {
    return (lhs | static_cast<int>(rhs));
}

inline int& operator|=(int& lhs, const Texture::eType& rhs) {
    return (lhs = (lhs | static_cast<int>(rhs)));
}

inline int operator&(int& lhs, const Texture::eType& rhs) {
    return (lhs & static_cast<int>(rhs));
}

inline int& operator&=(int& lhs, const Texture::eType& rhs) {
    return (lhs = (lhs & static_cast<int>(rhs)));
}
