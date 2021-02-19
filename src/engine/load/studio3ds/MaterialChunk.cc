using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimpleEngine.load.studio3ds
{
    struct MaterialChunk
    {
        StringChunk name;
        ColorChunk ambient;
        ColorChunk diffuse;
        ColorChunk specular;
        StringChunk texture;

        MaterialChunk(BinaryChunk c)
        {
            if (c.id != ChunkId.MATERIAL_BLOCK)
                throw Exception("Not a material chunk");
            std::vector<BinaryChunk> chunks = c.SubChunks;

            name = StringChunk(BinaryChunk.SelectChunk(ChunkId.MATERIAL_NAME, chunks));
            ambient = ColorChunk(BinaryChunk.SelectChunkOrNull(ChunkId.AMBIENT_COLOR, chunks));
            diffuse = ColorChunk(BinaryChunk.SelectChunkOrNull(ChunkId.DIFFUSE_COLOR, chunks));
            specular = ColorChunk(BinaryChunk.SelectChunkOrNull(ChunkId.SPECULAR_COLOR, chunks));
            texture = StringChunk(BinaryChunk.SelectChunk(ChunkId.TEXTURE_MAP_1, chunks).getSub(ChunkId.MAPPING_FILENAME));
        }
    }
}
