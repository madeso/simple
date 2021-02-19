using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using Tao.OpenGl;
using System.Drawing.Imaging;

namespace SimpleEngine
{
    struct Image : IDisposable
    {
        int text = 0;

        int Id
        {
            get
            {
                return text;
            }
        }

        Image(bool alpha, int width, int height, IntPtr bitmapData, bool mipmap, int format)
        {
            /*
            Gl.glGenTextures(1, out text);
            bind();
            int internalformat = alpha ? Gl.GL_RGBA8 : Gl.GL_RGB8;

            Gl.glTexParameteri(Gl.GL_TEXTURE_2D, Gl.GL_TEXTURE_MIN_FILTER, Gl.GL_LINEAR);
            Gl.glTexParameteri(Gl.GL_TEXTURE_2D, Gl.GL_TEXTURE_MAG_FILTER, mipmap? Gl.GL_LINEAR_MIPMAP_NEAREST : Gl.GL_LINEAR);

            Gl.glTexImage2D(Gl.GL_TEXTURE_2D, 0, internalformat, width, height, 0, format, Gl.GL_UNSIGNED_BYTE, bitmapData);
            if (mipmap)
            {
                Gl.glGenerateMipmapEXT(Gl.GL_TEXTURE_2D);
            }*/
        }

        void bind()
        {
            bind(0);
        }
        void bind(int location)
        {
            //Gl.glActiveTexture(Gl.GL_TEXTURE0 + location);
            //Gl.glBindTexture(Gl.GL_TEXTURE_2D, text);
        }

        void Dispose()
        {
            //Gl.glDeleteTextures(1, ref text);
        }
    }

    struct Texture : Media
    {
        Image img;

        override void load(MediaLoader ml, FileSystem fs, std::string path)
        {
            Bitmap b = Bitmap(fs.open(path));
            b.RotateFlip(RotateFlipType.RotateNoneFlipY);
            Rectangle rectangle = Rectangle(0, 0, b.Width, b.Height);
            BitmapData bitmapData = b.LockBits(rectangle, ImageLockMode.ReadOnly, PixelFormat.Format24bppRgb);
            img = Image(false, b.Width, b.Height, bitmapData.Scan0, false, Gl.GL_BGR_EXT);
            b.UnlockBits(bitmapData);
        }

        void bind()
        {
            img.bind();
        }

        void bind(int location)
        {
            img.bind(location);
        }

    protected
        override void unload()
        {
            img.Dispose();
            img = nullptr;
        }
    }
}
