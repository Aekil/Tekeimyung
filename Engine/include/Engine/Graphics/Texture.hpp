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
        AMBIENT = 2,
        BLOOM = 3,
        BLOOM_ALPHA = 4
    };

public:
    Texture();
    Texture(int width, int height);
    ~Texture();

    static std::unique_ptr<Texture>         create(GLsizei width,
                                            GLsizei height,
                                            GLint internalFormat = GL_RGBA,
                                            GLint format = GL_RGBA,
                                            GLenum type = GL_UNSIGNED_BYTE,
                                            GLint minFilter = GL_LINEAR,
                                            GLint maxFilter = GL_LINEAR,
                                            GLint wrapS = GL_CLAMP_TO_EDGE,
                                            GLint wrapT = GL_CLAMP_TO_EDGE);

    bool                                    loadFromFile(const std::string& fileName) override final;
    void                                    load(GLsizei width,
                                                GLsizei height,
                                                GLint internalFormat = GL_RGBA,
                                                GLint format = GL_RGBA,
                                                GLenum type = GL_UNSIGNED_BYTE,
                                                unsigned char* data = nullptr,
                                                GLint minFilter = GL_LINEAR,
                                                GLint maxFilter = GL_LINEAR,
                                                GLint wrapS = GL_REPEAT,
                                                GLint wrapT = GL_REPEAT);

    void                                    bind(GLenum unit = GL_TEXTURE0) const;
    void                                    unBind() const;

    unsigned int                            getWidth() const;
    unsigned int                            getHeight() const;
    unsigned char*                          getData() const;
    int                                     getComponentsNumber() const;
    GLuint                                  getNative();

    void                                    generateMipmaps(uint32_t levels);

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
