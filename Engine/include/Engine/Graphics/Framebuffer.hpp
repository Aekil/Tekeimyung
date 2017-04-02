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

    Framebuffer(Framebuffer&& framebuffer);
    Framebuffer& operator=(Framebuffer&& framebuffer);

    void                        bind(GLenum target) const;
    void                        unBind(GLenum target) const;
    void                        use(GLenum target) const;

    void                        removeColorAttachments();

    void                        addColorAttachment(std::unique_ptr<Texture> texture);
    void                        setDepthAttachment(GLenum internalformat, GLsizei width, GLsizei height);

    bool                        isComplete() const;

    const std::vector<std::unique_ptr<Texture> >& getColorAttachments() const;

private:
    void                        setAttachment(GLuint attachmentId, std::unique_ptr<Texture>& texture);

private:
    GLuint                      _fbo;

    std::vector<std::unique_ptr<Texture> > _colorAttachments;
    std::vector<GLuint >        _colorAttachmentsIds;
    GLuint                      _depthBuffer;

    bool                        _hasDepthBuffer{false};
};
