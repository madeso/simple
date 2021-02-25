#pragma once

#include <functional>

#include "engine/texture.h"

namespace SimpleEngine
{
    struct RenderBuffer
    {
        unsigned int buffer;

        unsigned int Buffer() const;

        RenderBuffer(int internalFormat, int width, int height);

        void bind();
    };

    struct Size
    {
        Size();
        Size(int w, int h);

        int Width;
        int Height;

        bool operator<(const Size& other) const;

        std::string ToString() const;

        bool operator==(const Size& rhs) const;
    };

    struct Fbo
    {
        unsigned int fbo;
        int width;
        int height;
        bool mipmap;

        int Width() const;

        int Height() const;

        Size GetSize() const;

        std::shared_ptr<RenderBuffer> depth;
        std::shared_ptr<Texture> texture;

        Fbo(int width, int height, bool mipmap);

        void attach(std::shared_ptr<RenderBuffer> b, int attachmentPoint);

        void attach(std::shared_ptr<Texture> img, int attachmentPoint);

        void updateTexture(std::function<void()> renderer);

        void bindTexture();

        void bindTexture(int location);
    };

    void Bind(Fbo* a);
}
