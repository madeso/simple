using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimpleEngine.load.studio3ds
{
    class LocalCoordinateSystemChunk
    {
        public readonly mat44 mat;
        public LocalCoordinateSystemChunk(BinaryChunk c)
        {
            if (c.id != ChunkId.LOCAL_COORDINATES_SYSTEM) throw new Exception("not a coordsys");

            Binary b = c.Binary;
            float[,] lmat = new float[4, 4];

            for (int j = 0; j < 4; j++)
            {
                for (int i = 0; i < 3; i++)
                {
                    lmat[i, j] = b.rfloat();
                }
            }
            lmat[0,3] = 0;
            lmat[1,3] = 0;
            lmat[2,3] = 0;
            lmat[3,3] = 1;
            mat = mat44.FromRowMajor(new float[] {  lmat[0, 0], lmat[1, 0], lmat[2, 0], lmat[3, 0],
                                                    lmat[0, 1], lmat[1, 1], lmat[2, 1], lmat[3, 1],
                                                    lmat[0, 2], lmat[1, 2], lmat[2, 2], lmat[3, 2],
                                                    lmat[0, 3], lmat[1, 3], lmat[2, 3], lmat[3, 3] });
        }

        internal vec3 translate(vec3 p)
        {
            return mat * p;
        }
    }
}
