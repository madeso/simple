using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimpleEngine.load.studio3ds
{
    class MainChunk
    {
        public readonly Editor3dChunk editor;
        public MainChunk(BinaryChunk c)
        {
            if (c.id != ChunkId.MAIN_CHUNK) throw new Exception("Not a main chunk");
            BinaryChunk ed = c.getSub(ChunkId.EDITOR_3D_CHUNK);
            if (ed == null) throw new Exception("Missing 3d editor chunk");
            editor = new Editor3dChunk(ed);
        }
    }
}
