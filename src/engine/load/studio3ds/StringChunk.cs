using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimpleEngine.load.studio3ds
{
    class StringChunk
    {
        public readonly string Value;
        public StringChunk(BinaryChunk c)
        {
            Binary b = c.Binary;
            Value = b.ReadString();
        }

        public override string ToString()
        {
            return Value;
        }
    }
}
