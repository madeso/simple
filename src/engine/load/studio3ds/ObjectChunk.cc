using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimpleEngine.load.studio3ds
{
    struct ObjectChunk
    {
        TriMeshChunk trimesh;
        std::string name;

        ObjectChunk(BinaryChunk c)
        {
            if (c.id != ChunkId.OBJECT_BLOCK)
                throw Exception("Not a object block");

            std::vector<BinaryChunk> chunks = std::vector<BinaryChunk>();
            Binary b = Binary(c.bytes);
            name = b.ReadString();
            BinaryChunk.ReadFromBinary(chunks, b);
            BinaryChunk tm = BinaryChunk.SelectChunk(ChunkId.TRIANGULAR_MESH, chunks);
            if (tm == nullptr)
                throw Exception("Missing a trimesh chunk");
            trimesh = TriMeshChunk(tm);
        }
    }
}
