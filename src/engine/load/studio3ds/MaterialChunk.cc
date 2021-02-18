using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimpleEngine.load.studio3ds
{
    class MaterialChunk
    {
        public readonly StringChunk name;
        public readonly ColorChunk ambient;
        public readonly ColorChunk diffuse;
        public readonly ColorChunk specular;
        public readonly StringChunk texture;


        public MaterialChunk(BinaryChunk c)
        {
            if (c.id != ChunkId.MATERIAL_BLOCK) throw new Exception("Not a material chunk");
            List<BinaryChunk> chunks = c.SubChunks;

            name = new StringChunk(BinaryChunk.SelectChunk(ChunkId.MATERIAL_NAME, chunks));
            ambient = new ColorChunk(BinaryChunk.SelectChunkOrNull(ChunkId.AMBIENT_COLOR, chunks));
            diffuse = new ColorChunk(BinaryChunk.SelectChunkOrNull(ChunkId.DIFFUSE_COLOR, chunks));
            specular = new ColorChunk(BinaryChunk.SelectChunkOrNull(ChunkId.SPECULAR_COLOR, chunks));
            texture = new StringChunk(BinaryChunk.SelectChunk(ChunkId.TEXTURE_MAP_1, chunks).getSub( ChunkId.MAPPING_FILENAME) );
        }
    }
}
