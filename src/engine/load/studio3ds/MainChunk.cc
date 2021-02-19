using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimpleEngine.load.studio3ds
{
    struct MainChunk
    {
        Editor3dChunk editor;
        MainChunk(BinaryChunk c)
        {
            if (c.id != ChunkId.MAIN_CHUNK)
                throw std::runtime_error("Not a main chunk");
            BinaryChunk ed = c.getSub(ChunkId.EDITOR_3D_CHUNK);
            if (ed == nullptr)
                throw std::runtime_error("Missing 3d editor chunk");
            editor = Editor3dChunk(ed);
        }
    }
}
