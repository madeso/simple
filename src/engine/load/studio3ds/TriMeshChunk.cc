using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimpleEngine.load.studio3ds
{
    class TriMeshChunk
    {
        public class Poly
        {
            public Poly(int a, int b, int c)
            {
                this.a = a;
                this.b = b;
                this.c = c;
            }

            public override string ToString()
            {
                return string.Format("{0}, {1}, {2}", a, b, c);
            }

            public readonly int a;
            public readonly int b;
            public readonly int c;
        }

        public readonly List<vec3> points;
        public readonly List<Poly> faces;
        public readonly List<vec2> mapping;
        public readonly List<FaceMaterialChunk> facematerials;
        public readonly LocalCoordinateSystemChunk coordsys;

        public TriMeshChunk(BinaryChunk c)
        {
            if (c.id != ChunkId.TRIANGULAR_MESH) throw new Exception("Not a trimesh");
            List<BinaryChunk> chunks = c.SubChunks;
            points = ParsePoints( BinaryChunk.SelectChunk(ChunkId.VERTICES_LIST, chunks) );
            mapping = ParseMapping(BinaryChunk.SelectChunk(ChunkId.MAPPING_COORDINATES_LIST, chunks));
            List<BinaryChunk> facechunks;
            faces = ParseFaces(BinaryChunk.SelectChunk(ChunkId.FACES_DESCRIPTION, chunks), out facechunks);
            List<FaceMaterialChunk> facematerials = new List<FaceMaterialChunk>();
            foreach (BinaryChunk bc in BinaryChunk.IterateChunks(ChunkId.FACES_MATERIAL, facechunks))
            {
                facematerials.Add( new FaceMaterialChunk(bc) );
            }
            this.facematerials = facematerials;
            coordsys = new LocalCoordinateSystemChunk(BinaryChunk.SelectChunk(ChunkId.LOCAL_COORDINATES_SYSTEM, chunks));
        }

        private List<Poly> ParseFaces(BinaryChunk c, out List<BinaryChunk> chunks)
        {
            Binary b = c.Binary;
            int facecount = b.int2();

            List<Poly> res = new List<Poly>();
            for (int i = 0; i < facecount; ++i)
            {
                int x = b.int2();
                int y = b.int2();
                int z = b.int2();
                int flags = b.int2();
                res.Add(new Poly(x, y, z));
            }
            chunks = new List<BinaryChunk>();
            BinaryChunk.ReadFromBinary(chunks, b);

            return res;
        }

        private List<vec2> ParseMapping(BinaryChunk c)
        {
            Binary b = c.Binary;
            int mapcount = b.int2();

            List<vec2> res = new List<vec2>();
            for (int i = 0; i < mapcount; ++i)
            {
                float x = b.rfloat();
                float y = b.rfloat();
                res.Add(new vec2(x, y));
            }

            return res;
        }

        private List<vec3> ParsePoints(BinaryChunk c)
        {
            Binary b = c.Binary;
            int points = b.int2();

            List<vec3> res = new List<vec3>();
            for (int i = 0; i < points; ++i)
            {
                float x = b.rfloat();
                float z = -b.rfloat();
                float y = b.rfloat();
                res.Add(new vec3(x, y, z));
            }

            return res;
        }
    }
}
