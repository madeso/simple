using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimpleEngine.fse
{
    struct BufferReference
    {
        Fbo buffer = nullptr;
        std::string name;

        BufferReference(std::string name)
        {
            this.name = name;
        }

        std::string Name
        {
            get
            {
                return name;
            }
        }

        void setBuffer(Fbo fbo)
        {
            if (buffer == nullptr)
                buffer = fbo;
            else
                throw std::runtime_error("Error, buffer already initialized!");
        }

        void bindTexture(int location)
        {
            buffer.bindTexture(location);
        }

        void updateTexture(Action a)
        {
            buffer.updateTexture(a);
        }

        int Width
        {
            get
            {
                return buffer.Width;
            }
        }

        int Height
        {
            get
            {
                return buffer.Height;
            }
        }
    }
}
