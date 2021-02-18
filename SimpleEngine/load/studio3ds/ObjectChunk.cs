using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimpleEngine.load.studio3ds
{
    class ObjectChunk
    {
        public readonly TriMeshChunk trimesh;
        public readonly string name;

        public ObjectChunk(BinaryChunk c)
        {
            if (c.id != ChunkId.OBJECT_BLOCK) throw new Exception("Not a object block");
            
            List<BinaryChunk> chunks = new List<BinaryChunk>();
            Binary b = new Binary(c.bytes);
            name = b.ReadString();
            BinaryChunk.ReadFromBinary(chunks, b);
            BinaryChunk tm = BinaryChunk.SelectChunk(ChunkId.TRIANGULAR_MESH, chunks);
            if (tm == null) throw new Exception("Missing a trimesh chunk");
            trimesh = new TriMeshChunk(tm);
        }
    }
}
