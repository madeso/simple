#include "engine/fbo.h"

#include <stdexcept>


#include "engine/opengl.h"
#include "fmt/core.h"

namespace SimpleEngine
{
    unsigned int RenderBuffer::Buffer() const
    {
        return buffer;
    }

    RenderBuffer::RenderBuffer(int internalFormat, int width, int height)
    {
        glGenRenderbuffers(1, &buffer);

        bind();
        glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, width, height);
    }

    void RenderBuffer::bind()
    {
        glBindRenderbuffer(GL_RENDERBUFFER, buffer);
    }

    Size::Size()
        : Width(512)
        , Height(512)
    {
    }

    Size::Size(int w, int h)
        : Width(w)
        , Height(h)
    {
    }

    bool Size::operator<(const Size& other) const
    {
        if (Width == other.Width)
        {
            return Height < other.Height;
        }
        else
        {
            return Width < other.Width;
        }
    }

    std::string Size::ToString() const
    {
        return fmt::format("{}x{}", Width, Height);
    }

    bool Size::operator==(const Size& rhs) const
    {
        return Width == rhs.Width && Height == rhs.Height;
    }

    int Fbo::Width() const
    {
        return width;
    }

    int Fbo::Height() const
    {
        return height;
    }

    Size Fbo::GetSize() const
    {
        return Size{Width(), Height()};
    }

    Fbo::Fbo(int width, int height, bool mipmap)
        : width(width)
        , height(height)
        , mipmap(mipmap)
    {
        glGenFramebuffers(1, &fbo);
        Bind(this);

        depth = std::make_shared<RenderBuffer>(GL_DEPTH_COMPONENT, width, height);
        attach(depth, GL_DEPTH_ATTACHMENT);

        texture = std::make_shared<Texture>(true, width, height, nullptr, mipmap, GL_RGBA);
        attach(texture, GL_COLOR_ATTACHMENT0);

        int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (status != GL_FRAMEBUFFER_COMPLETE)
        {
            throw std::runtime_error(fmt::format("Something went wrong when creating framebuffer: {}", status));
        }

        Bind(nullptr);
    }

    void Fbo::attach(std::shared_ptr<RenderBuffer> b, int attachmentPoint)
    {
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachmentPoint, GL_RENDERBUFFER, b->Buffer());
    }

    void Fbo::attach(std::shared_ptr<Texture> img, int attachmentPoint)
    {
        const int mipmaplevel = 0;
        glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentPoint, GL_TEXTURE_2D, img->GetId(), mipmaplevel);
    }

    void Fbo::updateTexture(std::function<void()> renderer)
    {
        Bind(this);
        glPushAttrib(GL_VIEWPORT_BIT);
        glViewport(0, 0, width, height);

        renderer();

        glPopAttrib();
        Bind(nullptr);
    }

    void Fbo::bindTexture()
    {
        texture->Bind();
    }

    void Fbo::bindTexture(int location)
    {
        texture->Bind(location);
    }

    namespace
    {
        Fbo* bound = nullptr;
    }

    void Bind(Fbo* a)
    {
        if (bound != nullptr && a != nullptr)
            throw std::runtime_error("Already bound a fbo");
        bound = a;
        int fbo = a != nullptr ? a->fbo : 0;
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    }
}
