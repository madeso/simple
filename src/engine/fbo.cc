using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Tao.OpenGl;

namespace SimpleEngine
{
    struct RenderBuffer
    {
        int buffer;

        int Buffer
        {
            get
            {
                return buffer;
            }
        }

        RenderBuffer(int internalFormat, int width, int height)
        {
            Gl.glGenRenderbuffersEXT(1, out buffer);

            bind();
            Gl.glRenderbufferStorageEXT(Gl.GL_RENDERBUFFER_EXT, internalFormat, width, height);
        }

        void bind()
        {
            Gl.glBindRenderbufferEXT(Gl.GL_RENDERBUFFER_EXT, buffer);
        }
    }

    struct Size : IComparable<Size>
    {
        Size()
        {
            Width = 512;
            Height = 512;
        }
        int Width
        {
            get;
            set;
        }
        int Height
        {
            get;
            set;
        }

        int CompareTo(Size other)
        {
            if (this.Width == other.Width)
            {
                return this.Height.CompareTo(other.Height);
            }
            else
            {
                return this.Width.CompareTo(other.Width);
            }
        }

        override std::string ToString()
        {
            return Width.ToString() + "x" + Height.ToString();
        }

        struct Comperator : IEqualityComparer<Size>
        {
            bool Equals(Size x, Size y)
            {
                return x.CompareTo(y) == 0;
            }

            int GetHashCode(Size s)
            {
                return s.Width * 3 + s.Height;
            }
        }
    }

    struct Fbo
    {
        int fbo;

        int width;

        int height;

        bool mipmap;

        int Width
        {
            get
            {
                return width;
            }
        }

        int Height
        {
            get
            {
                return height;
            }
        }

        Size Size
        {
            get
            {
                return Size{Width = Width, Height = Height};
            }
        }

        RenderBuffer depth;
        Image texture;

        Fbo(int width, int height, bool mipmap)
        {
            this.width = width;
            this.height = height;
            this.mipmap = mipmap;

            Gl.glGenFramebuffersEXT(1, out fbo);
            Bind(this);

            depth = RenderBuffer(Gl.GL_DEPTH_COMPONENT, width, height);
            attach(depth, Gl.GL_DEPTH_ATTACHMENT_EXT);

            texture = Image(true, width, height, IntPtr.Zero, mipmap, Gl.GL_RGBA);
            attach(texture, Gl.GL_COLOR_ATTACHMENT0_EXT);

            int status = Gl.glCheckFramebufferStatusEXT(Gl.GL_FRAMEBUFFER_EXT);
            if (status != Gl.GL_FRAMEBUFFER_COMPLETE_EXT)
                throw std::runtime_error("Something went wrong when creating framebuffer: " + status.ToString());

            Bind(nullptr);
        }

        void attach(RenderBuffer b, int attachmentPoint)
        {
            Gl.glFramebufferRenderbufferEXT(Gl.GL_FRAMEBUFFER_EXT, attachmentPoint, Gl.GL_RENDERBUFFER_EXT, b.Buffer);
        }

        void attach(Image img, int attachmentPoint)
        {
            const int mipmaplevel = 0;
            Gl.glFramebufferTexture2DEXT(Gl.GL_FRAMEBUFFER_EXT, attachmentPoint, Gl.GL_TEXTURE_2D, img.Id, mipmaplevel);
        }

        static Fbo bound = nullptr;

        static void Bind(Fbo a)
        {
            if (bound != nullptr && a != nullptr)
                throw std::runtime_error("Already bound a fbo");
            bound = a;
            int fbo = a != nullptr ? a.fbo : 0;
            Gl.glBindFramebufferEXT(Gl.GL_FRAMEBUFFER_EXT, fbo);
        }

        void updateTexture(Action renderer)
        {
            Bind(this);
            Gl.glPushAttrib(Gl.GL_VIEWPORT_BIT);
            Gl.glViewport(0, 0, width, height);

            renderer();

            Gl.glPopAttrib();
            Bind(nullptr);
        }

        void bindTexture()
        {
            texture.bind();
        }

        void bindTexture(int location)
        {
            texture.bind(location);
        }
    }
}
