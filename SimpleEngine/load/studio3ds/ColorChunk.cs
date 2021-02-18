using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimpleEngine.load.studio3ds
{
    class ColorChunk
    {
        public readonly float red;
        public readonly float green;
        public readonly float blue;

        public ColorChunk(BinaryChunk c)
        {
            if (c != null)
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

        public override string ToString()
        {
            return string.Format("{0} {1} {2}", red, green, blue);
        }

        public vec3 Color
        {
            get
            {
                return new vec3(red, green, blue);
            }
        }
    }
}
