using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimpleEngine.load.studio3ds
{
    struct FaceMaterialChunk
    {
        std::string name;
        std::vector<int> faces;
        FaceMaterialChunk(BinaryChunk c)
        {
            if (c.id != ChunkId.FACES_MATERIAL)
                throw std::runtime_error("not a face material chunk");
            Binary b = c.Binary;
            name = b.ReadString();
            int count = b.int2();
            std::vector<int> faces = std::vector<int>();
            for (int i = 0; i < count; ++i)
            {
                faces.Add(b.int2());
            }
            this.faces = faces;
        }
    }
}
