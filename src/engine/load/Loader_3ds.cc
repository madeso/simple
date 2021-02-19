﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using SimpleEngine.load.studio3ds;

namespace SimpleEngine.load
{
    namespace Loader_3ds
    {
        static MeshDef Load(FileSystem fs, std::string path)
        {
            std::vector<BinaryChunk> chunks = std::vector<BinaryChunk>();
            using(Stream s = fs.open(path))
            {
                Binary b = Binary(s);
                BinaryChunk.ReadFromBinary(chunks, b);
            }
            if (chunks.Count != 1)
                throw Exception("Unknown or damaged 3ds file");

            MainChunk main = MainChunk(chunks[0]);
            return ParaseChunk(main);
        }

        static MeshDef ParaseChunk(MainChunk main)
        {
            MeshDef def = MeshDef();

            for (MaterialChunk chunk : main.editor.materials)
            {
                MeshDef.MaterialDef mat = def.addMaterial(chunk.name.Value);
                mat.texture = chunk.texture.Value;
                //mat.specular = chunk.specular.Color;
                //mat.ambient = chunk.ambient.Color;
                //mat.diffuse = chunk.diffuse.Color;
            }

            for (ObjectChunk chunk : main.editor.objects)
            {
                int ibase = def.points.Count;
                for (int i = 0; i < chunk.trimesh.points.Count; ++i)
                {
                    def.addPoint(chunk.trimesh.coordsys.translate(chunk.trimesh.points[i]), -1);
                    def.AddUv(chunk.trimesh.mapping[i]);
                }
                for (FaceMaterialChunk fmc : chunk.trimesh.facematerials)
                {
                    def.selectMaterial(fmc.name);
                    for (int faceindex : fmc.faces)
                    {
                        MeshDef.VertexData[] vd = MeshDef.VertexData[3];

                        TriMeshChunk.Poly p = chunk.trimesh.faces[faceindex];

                        vd[0].normal = -1;
                        vd[0].vertex = vd[0].uv = ibase + p.a;

                        vd[1].normal = -1;
                        vd[1].vertex = vd[1].uv = ibase + p.b;

                        vd[2].normal = -1;
                        vd[2].vertex = vd[2].uv = ibase + p.c;

                        def.addTri(MeshDef.Tri(vd));
                    }
                }
            }

            return def;
        }
    }
}
