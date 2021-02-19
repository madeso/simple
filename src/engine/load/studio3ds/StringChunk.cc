using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimpleEngine.load.studio3ds
{
    struct StringChunk
    {
        std::string Value;
        StringChunk(BinaryChunk c)
        {
            Binary b = c.Binary;
            Value = b.ReadString();
        }

        override std::string ToString()
        {
            return Value;
        }
    }
}
