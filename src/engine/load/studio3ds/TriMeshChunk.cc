using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimpleEngine.load.studio3ds
{
    struct TriMeshChunk
    {
        struct Poly
        {
            Poly(int a, int b, int c)
            {
                this.a = a;
                this.b = b;
                this.c = c;
            }

            override std::string ToString()
            {
                return fmt::format("{0}, {1}, {2}", a, b, c);
            }

            int a;
            int b;
            int c;
        }

        std::vector<vec3>
            points;
        std::vector<Poly> faces;
        std::vector<vec2> mapping;
        std::vector<FaceMaterialChunk> facematerials;
        LocalCoordinateSystemChunk coordsys;

        TriMeshChunk(BinaryChunk c)
        {
            if (c.id != ChunkId.TRIANGULAR_MESH)
                throw std::runtime_error("Not a trimesh");
            std::vector<BinaryChunk> chunks = c.SubChunks;
            points = ParsePoints(BinaryChunk.SelectChunk(ChunkId.VERTICES_LIST, chunks));
            mapping = ParseMapping(BinaryChunk.SelectChunk(ChunkId.MAPPING_COORDINATES_LIST, chunks));
            std::vector<BinaryChunk> facechunks;
            faces = ParseFaces(BinaryChunk.SelectChunk(ChunkId.FACES_DESCRIPTION, chunks), out facechunks);
            std::vector<FaceMaterialChunk> facematerials = std::vector<FaceMaterialChunk>();
            for (BinaryChunk bc : BinaryChunk.IterateChunks(ChunkId.FACES_MATERIAL, facechunks))
            {
                facematerials.Add(FaceMaterialChunk(bc));
            }
            this.facematerials = facematerials;
            coordsys = LocalCoordinateSystemChunk(BinaryChunk.SelectChunk(ChunkId.LOCAL_COORDINATES_SYSTEM, chunks));
        }

        std::vector<Poly> ParseFaces(BinaryChunk c, out std::vector<BinaryChunk> chunks)
        {
            Binary b = c.Binary;
            int facecount = b.int2();

            std::vector<Poly> res = std::vector<Poly>();
            for (int i = 0; i < facecount; ++i)
            {
                int x = b.int2();
                int y = b.int2();
                int z = b.int2();
                int flags = b.int2();
                res.Add(Poly(x, y, z));
            }
            chunks = std::vector<BinaryChunk>();
            BinaryChunk.ReadFromBinary(chunks, b);

            return res;
        }

        std::vector<vec2> ParseMapping(BinaryChunk c)
        {
            Binary b = c.Binary;
            int mapcount = b.int2();

            std::vector<vec2> res = std::vector<vec2>();
            for (int i = 0; i < mapcount; ++i)
            {
                float x = b.rfloat();
                float y = b.rfloat();
                res.Add(vec2(x, y));
            }

            return res;
        }

        std::vector<vec3> ParsePoints(BinaryChunk c)
        {
            Binary b = c.Binary;
            int points = b.int2();

            std::vector<vec3> res = std::vector<vec3>();
            for (int i = 0; i < points; ++i)
            {
                float x = b.rfloat();
                float z = -b.rfloat();
                float y = b.rfloat();
                res.Add(vec3(x, y, z));
            }

            return res;
        }
    }
}
