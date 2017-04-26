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
    if (_hasDepthBuffer)
    {
        glDeleteRenderbuffers(1, &_depthBuffer);
    }

    glDeleteFramebuffers(1, &_fbo);
}

Framebuffer::Framebuffer(Framebuffer&& framebuffer)
{
    _fbo = framebuffer._fbo;
    _colorAttachments = std::move(framebuffer._colorAttachments);
    _colorAttachmentsIds = framebuffer._colorAttachmentsIds;
    _depthBuffer = framebuffer._depthBuffer;
    _hasDepthBuffer = framebuffer._hasDepthBuffer;
}

Framebuffer& Framebuffer::operator=(Framebuffer&& framebuffer)
{
    _fbo = framebuffer._fbo;
    _colorAttachments = std::move(framebuffer._colorAttachments);
    _colorAttachmentsIds = framebuffer._colorAttachmentsIds;
    _depthBuffer = framebuffer._depthBuffer;
    _hasDepthBuffer = framebuffer._hasDepthBuffer;
    return (*this);
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
    if (target == GL_DRAW_FRAMEBUFFER || target == GL_FRAMEBUFFER)
    {
        glDrawBuffers((GLsizei)_colorAttachmentsIds.size(), _colorAttachmentsIds.data());
    }
}

void    Framebuffer::removeColorAttachments()
{
    _colorAttachmentsIds.clear();
    _colorAttachments.clear();
}

void    Framebuffer::addColorAttachment(std::unique_ptr<Texture> texture)
{
    GLuint colorAttachmentId = GL_COLOR_ATTACHMENT0 + (GLuint)_colorAttachments.size();

    setAttachment(colorAttachmentId, texture);

    _colorAttachmentsIds.push_back(colorAttachmentId);
    _colorAttachments.push_back(std::move(texture));
}

void    Framebuffer::setDepthAttachment(GLenum internalformat, GLsizei width, GLsizei height)
{
    bind(GL_FRAMEBUFFER);

    if (!_hasDepthBuffer)
    {
        glGenRenderbuffers(1, &_depthBuffer);
        _hasDepthBuffer = true;
    }


    glBindRenderbuffer(GL_RENDERBUFFER, _depthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, internalformat, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthBuffer);
}

void    Framebuffer::setAttachment(GLuint attachmentId, std::unique_ptr<Texture>& texture)
{
    texture->bind();

    bind(GL_FRAMEBUFFER);
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentId, GL_TEXTURE_2D, texture->getNative(), 0);
}

bool    Framebuffer::isComplete() const
{
    return (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
}

const std::vector<std::unique_ptr<Texture> >&   Framebuffer::getColorAttachments() const
{
    return (_colorAttachments);
}

std::vector<std::unique_ptr<Texture> >&   Framebuffer::getColorAttachments()
{
    return (_colorAttachments);
}
