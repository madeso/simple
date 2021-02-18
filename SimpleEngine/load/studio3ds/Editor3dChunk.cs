using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimpleEngine.load.studio3ds
{
    class Editor3dChunk
    {
        public readonly List<ObjectChunk> objects;
        public readonly List<MaterialChunk> materials;
        public Editor3dChunk(BinaryChunk c)
        {
            if (c.id != ChunkId.EDITOR_3D_CHUNK) throw new Exception("Not a 3d editor chunk");
            List<BinaryChunk> chunks = c.SubChunks;

            List<ObjectChunk> objects = new List<ObjectChunk>();
            List<MaterialChunk> materials = new List<MaterialChunk>();

            foreach (BinaryChunk oc in BinaryChunk.IterateChunks(ChunkId.OBJECT_BLOCK, chunks))
            {
                objects.Add( new ObjectChunk(oc) );
            }
            foreach (BinaryChunk mc in BinaryChunk.IterateChunks(ChunkId.MATERIAL_BLOCK, chunks))
            {
                materials.Add(new MaterialChunk(mc));
            }

            this.objects = objects;
            this.materials = materials;
        }
    }
}
