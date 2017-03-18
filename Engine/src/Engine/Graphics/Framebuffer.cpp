/**
* @Author   Guillaume Labey
*/

#include <Engine/Graphics/Framebuffer.hpp>

Framebuffer::Framebuffer()
{
    glGenFramebuffers(1, &_fbo);
}

Framebuffer::~Framebuffer()
{
    glDeleteFramebuffers(1, &_fbo);
}

void    Framebuffer::bind(GLenum target) const
{
    glBindFramebuffer(target, _fbo);
}

void    Framebuffer::unBind(GLenum target) const
{
    glBindFramebuffer(target, 0);
}

void    Framebuffer::use(GLenum target) const
{
    bind(target);

    // Specify the color attachments to be used in fragment shader
    if (target == GL_DRAW_FRAMEBUFFER)
    {
        glDrawBuffers((GLsizei)_colorAttachments.size(), _colorAttachments.data());
    }
}

void    Framebuffer::addColorAttachment(Texture& texture)
{
    GLuint colorAttachment = GL_COLOR_ATTACHMENT0 + (GLuint)_colorAttachments.size();

    setAttachment(colorAttachment, texture);

    _colorAttachments.push_back(colorAttachment);
}

void    Framebuffer::setDepthAttachment(Texture& texture)
{
    setAttachment(GL_DEPTH_STENCIL_ATTACHMENT, texture);
}

void    Framebuffer::setAttachment(GLuint attachment, Texture& texture)
{
    texture.bind();

    glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, texture.getNative(), 0);
}

bool    Framebuffer::isComplete() const
{
    return (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
}
