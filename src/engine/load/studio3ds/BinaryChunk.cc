using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimpleEngine.load.studio3ds
{
    struct BinaryChunk
    {
        BinaryChunk(int id, byte[] bytes)
        {
            this.id = id;
            this.bytes = bytes;
        }
        int id;
        byte[] bytes;

        override std::string ToString()
        {
            return std::string.Format("{0} {1} byte(s)", ChunkId.ToString(id), bytes.Length);
        }

        static std::vector<BinaryChunk> Parse(byte[] bytes)
        {
            std::vector<BinaryChunk> res = std::vector<BinaryChunk>();
            Binary b = Binary(bytes);
            ReadFromBinary(res, b);
            return res;
        }

        Binary Binary
        {
            get
            {
                return Binary(bytes);
            }
        }

        std::vector<BinaryChunk> SubChunks
        {
            get
            {
                return Parse(bytes);
            }
        }

        BinaryChunk getSub(int id)
        {
            std::vector<BinaryChunk> s = SubChunks;
            return SelectChunk(id, s);
        }

        static BinaryChunk SelectChunk(int id, std::vector<BinaryChunk> s)
        {
            BinaryChunk c = SelectChunkOrNull(id, s);
            if (c == nullptr)
                throw Exception("Missing required chunk");
            else
                return c;
        }

        static IEnumerable<BinaryChunk> IterateChunks(int id, std::vector<BinaryChunk> s)
        {
            for (BinaryChunk c : s)
            {
                if (c.id == id)
                {
                    yield return c;
                }
            }
        }

        static BinaryChunk SelectChunkOrNull(int id, std::vector<BinaryChunk> s)
        {
            for (BinaryChunk c : s)
            {
                if (c.id == id)
                    return c;
            }
            return nullptr;
        }

        static void ReadFromBinary(std::vector<BinaryChunk> chunks, Binary b)
        {
            while (b.Continue)
            {
                int id = b.int2();
                int count = b.int4();
                byte[] bytes = b.Read(count - (2 + 4));
                chunks.Add(BinaryChunk(id, bytes));
            }
        }
    }
}
