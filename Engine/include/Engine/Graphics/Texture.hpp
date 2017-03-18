/**
* @Author   Guillaume Labey
*/

#pragma once

#include <GL/glew.h>
#include <string>
#include <memory>

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
    Texture(int width, int height);
    ~Texture();

    static std::unique_ptr<Texture>         create(GLsizei width,
                                            GLsizei height,
                                            GLint internalFormat = GL_RGB,
                                            GLint format = GL_RGB,
                                            GLenum type = GL_UNSIGNED_BYTE,
                                            GLint filter = GL_LINEAR,
                                            GLint wrap = GL_CLAMP_TO_EDGE);

    bool                                    loadFromFile(const std::string& fileName) override final;

    void                                    bind(GLenum unit = GL_TEXTURE0) const;
    void                                    unBind() const;

    unsigned int                            getWidth() const;
    unsigned int                            getHeight() const;
    unsigned char*                          getData() const;
    int                                     getComponentsNumber() const;
    GLuint                                  getNative();

    static Resource::eType      getResourceType() { return Resource::eType::TEXTURE; }

private:
    GLuint                                  _texture;
    int                                     _width;
    int                                     _height;
    unsigned char*                          _data{nullptr};

    // Components number
    int                                     _comp;

    GLenum                                  _unit;
};


inline int operator~(const Texture::eType& rhs) {
    return (~static_cast<int>(rhs));
}

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
