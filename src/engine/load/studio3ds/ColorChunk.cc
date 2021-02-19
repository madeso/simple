using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimpleEngine.load.studio3ds
{
    struct ColorChunk
    {
        float red;
        float green;
        float blue;

        ColorChunk(BinaryChunk c)
        {
            if (c != nullptr)
            {
                Binary b = c.Binary;
                red = b.rfloat();
                green = b.rfloat();
                blue = b.rfloat();
            }
            else
            {
                red = 1;
                green = 1;
                blue = 1;
            }
        }

        override std::string ToString()
        {
            return fmt::format("{0} {1} {2}", red, green, blue);
        }

        vec3 Color
        {
            get
            {
                return vec3(red, green, blue);
            }
        }
    }
}
