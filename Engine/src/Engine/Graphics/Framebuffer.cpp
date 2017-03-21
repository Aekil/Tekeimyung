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

Framebuffer::Framebuffer(Framebuffer&& framebuffer)
{
    _fbo = framebuffer._fbo;
    _colorAttachments = std::move(framebuffer._colorAttachments);
    _colorAttachmentsIds = framebuffer._colorAttachmentsIds;
    _depthStencilAttachment = std::move(framebuffer._depthStencilAttachment);
}

Framebuffer& Framebuffer::operator=(Framebuffer&& framebuffer)
{
    _fbo = framebuffer._fbo;
    _colorAttachments = std::move(framebuffer._colorAttachments);
    _colorAttachmentsIds = framebuffer._colorAttachmentsIds;
    _depthStencilAttachment = std::move(framebuffer._depthStencilAttachment);
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

void    Framebuffer::setDepthAttachment(std::unique_ptr<Texture> texture)
{
    setAttachment(GL_DEPTH_STENCIL_ATTACHMENT, texture);
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
