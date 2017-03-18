/**
* @Author   Guillaume Labey
*/

#pragma once

#include <GL/glew.h>
#include <vector>

#include <Engine/Graphics/Texture.hpp>

class Framebuffer
{
public:
    Framebuffer();
    ~Framebuffer();

    void                        bind(GLenum target) const;
    void                        unBind(GLenum target) const;
    void                        use(GLenum target) const;

    void                        removeColorAttachments();

    void                        addColorAttachment(Texture& texture);
    void                        setDepthAttachment(Texture& texture);

    bool                        isComplete() const;

private:
    void                        setAttachment(GLuint attachment, Texture& texture);

private:
    GLuint                      _fbo;

    std::vector<GLuint>         _colorAttachments;
};
