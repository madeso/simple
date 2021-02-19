using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimpleEngine.load.studio3ds
{
    struct Editor3dChunk
    {
        std::vector<ObjectChunk> objects;
        std::vector<MaterialChunk> materials;
        Editor3dChunk(BinaryChunk c)
        {
            if (c.id != ChunkId.EDITOR_3D_CHUNK)
                throw Exception("Not a 3d editor chunk");
            std::vector<BinaryChunk> chunks = c.SubChunks;

            std::vector<ObjectChunk> objects = std::vector<ObjectChunk>();
            std::vector<MaterialChunk> materials = std::vector<MaterialChunk>();

            for (BinaryChunk oc : BinaryChunk.IterateChunks(ChunkId.OBJECT_BLOCK, chunks))
            {
                objects.Add(ObjectChunk(oc));
            }
            for (BinaryChunk mc : BinaryChunk.IterateChunks(ChunkId.MATERIAL_BLOCK, chunks))
            {
                materials.Add(MaterialChunk(mc));
            }

            this.objects = objects;
            this.materials = materials;
        }
    }
}
