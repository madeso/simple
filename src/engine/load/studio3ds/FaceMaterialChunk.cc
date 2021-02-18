using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimpleEngine.load.studio3ds
{
    class FaceMaterialChunk
    {
        public readonly string name;
        public readonly List<int> faces;
        public FaceMaterialChunk(BinaryChunk c)
        {
            if (c.id != ChunkId.FACES_MATERIAL) throw new Exception("not a face material chunk");
            Binary b = c.Binary;
            name = b.ReadString();
            int count = b.int2();
            List<int> faces = new List<int>();
            for (int i = 0; i < count; ++i)
            {
                faces.Add(b.int2());
            }
            this.faces = faces;
        }
    }
}
