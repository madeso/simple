using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Tao.OpenGl;

namespace SimpleEngine
{

    internal class RenderBuffer
    {
        readonly int buffer;

        public int Buffer
        {
            get
            {
                return buffer;
            }
        }

        public RenderBuffer(int internalFormat, int width, int height)
        {
            Gl.glGenRenderbuffersEXT(1, out buffer);

            bind();
            Gl.glRenderbufferStorageEXT(Gl.GL_RENDERBUFFER_EXT, internalFormat, width, height);
        }

        private void bind()
        {
            Gl.glBindRenderbufferEXT(Gl.GL_RENDERBUFFER_EXT, buffer);
        }
    }

    public class Size : IComparable<Size>
    {
        public Size()
        {
            Width = 512;
            Height = 512;
        }
        public int Width { get; set; }
        public int Height { get; set; }

        public int CompareTo(Size other)
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

        public override string ToString()
        {
            return Width.ToString() + "x" + Height.ToString();
        }

        public class Comperator : IEqualityComparer<Size>
        {
            public bool Equals(Size x, Size y)
            {
                return x.CompareTo(y) == 0;
            }

            public int GetHashCode(Size s)
            {
                return s.Width*3 + s.Height;
            }
        }
    }

    public class Fbo
    {
        private readonly int fbo;
        
        private readonly int width;
        private readonly int height;
        private readonly bool mipmap;

        public int Width
        {
            get
            {
                return width;
            }
        }

        public int Height
        {
            get
            {
                return height;
            }
        }

        public Size Size
        {
            get
            {
                return new Size { Width= Width, Height= Height} ;
            }
        }

        RenderBuffer depth;
        Image texture;

        public Fbo(int width, int height, bool mipmap)
        {
            this.width = width;
            this.height = height;
            this.mipmap = mipmap;

            Gl.glGenFramebuffersEXT(1, out fbo);
            Bind(this);

            depth = new RenderBuffer(Gl.GL_DEPTH_COMPONENT, width, height);
            attach(depth, Gl.GL_DEPTH_ATTACHMENT_EXT);

            texture = new Image(true, width, height, IntPtr.Zero, mipmap, Gl.GL_RGBA);
            attach(texture, Gl.GL_COLOR_ATTACHMENT0_EXT);

            int status = Gl.glCheckFramebufferStatusEXT(Gl.GL_FRAMEBUFFER_EXT);
            if (status != Gl.GL_FRAMEBUFFER_COMPLETE_EXT) throw new Exception("Something went wrong when creating framebuffer: " + status.ToString());

            Bind(null);
        }

        private void attach(RenderBuffer b, int attachmentPoint)
        {
            Gl.glFramebufferRenderbufferEXT(Gl.GL_FRAMEBUFFER_EXT, attachmentPoint, Gl.GL_RENDERBUFFER_EXT, b.Buffer);
        }

        private void attach(Image img, int attachmentPoint)
        {
            const int mipmaplevel = 0;
            Gl.glFramebufferTexture2DEXT(Gl.GL_FRAMEBUFFER_EXT, attachmentPoint, Gl.GL_TEXTURE_2D, img.Id, mipmaplevel);
        }

        private static Fbo bound = null;
        private static void Bind(Fbo a)
        {
            if (bound != null && a != null) throw new Exception("Already bound a fbo");
            bound = a;
            int fbo = a != null ? a.fbo : 0;
            Gl.glBindFramebufferEXT(Gl.GL_FRAMEBUFFER_EXT, fbo);
        }

        public void updateTexture(Action renderer)
        {
            Bind(this);
            Gl.glPushAttrib(Gl.GL_VIEWPORT_BIT);
            Gl.glViewport(0, 0, width, height);
            
            renderer();

            Gl.glPopAttrib();
            Bind(null);
        }

        internal void bindTexture()
        {
            texture.bind();
        }

        internal void bindTexture(int location)
        {
            texture.bind(location);
        }
    }
}
