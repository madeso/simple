using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimpleEngine.load.studio3ds
{
    internal class BinaryChunk
    {
        public BinaryChunk(int id, byte[] bytes)
        {
            this.id = id;
            this.bytes = bytes;
        }
        public readonly int id;
        public readonly byte[] bytes;

        public override string ToString()
        {
            return string.Format("{0} {1} byte(s)", ChunkId.ToString(id), bytes.Length);
        }

        public static List<BinaryChunk> Parse(byte[] bytes)
        {
            List<BinaryChunk> res = new List<BinaryChunk>();
            Binary b = new Binary(bytes);
            ReadFromBinary(res, b);
            return res;
        }

        public Binary Binary
        {
            get
            {
                return new Binary(bytes);
            }
        }

        public List<BinaryChunk> SubChunks
        {
            get
            {
                return Parse(bytes);
            }
        }

        public BinaryChunk getSub(int id)
        {
            List<BinaryChunk> s = SubChunks;
            return SelectChunk(id, s);
        }

        public static BinaryChunk SelectChunk(int id, List<BinaryChunk> s)
        {
            BinaryChunk c = SelectChunkOrNull(id, s);
            if (c == null) throw new Exception("Missing required chunk");
            else return c;
        }

        public static IEnumerable<BinaryChunk> IterateChunks(int id, List<BinaryChunk> s)
        {
            foreach (BinaryChunk c in s)
            {
                if (c.id == id)
                {
                    yield return c;
                }
            }
        }

        public static BinaryChunk SelectChunkOrNull(int id, List<BinaryChunk> s)
        {
            foreach (BinaryChunk c in s)
            {
                if (c.id == id) return c;
            }
            return null;
        }

        public static void ReadFromBinary(List<BinaryChunk> chunks, Binary b)
        {
            while (b.Continue)
            {
                int id = b.int2();
                int count = b.int4();
                byte[] bytes = b.Read(count - (2 + 4));
                chunks.Add(new BinaryChunk(id, bytes));
            }
        }
    }
}
