using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimpleEngine.fse
{
    public class BufferReference
    {
        Fbo buffer = null;
        readonly string name;

        public BufferReference(string name)
        {
            this.name = name;
        }

        public string Name
        {
            get
            {
                return name;
            }
        }

        internal void setBuffer(Fbo fbo)
        {
            if (buffer == null) buffer = fbo;
            else throw new Exception("Error, buffer already initialized!");
        }

        internal void bindTexture(int location)
        {
            buffer.bindTexture(location);
        }

        internal void updateTexture(Action a)
        {
            buffer.updateTexture(a);
        }

        internal int Width
        {
            get
            {
                return buffer.Width;
            }
        }

        internal int Height
        {
            get
            {
                return buffer.Height;
            }
        }
    }
}
