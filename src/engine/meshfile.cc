using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace SimpleEngine
{
    public static class MeshFile
    {
        public static MeshDef Load(FileSystem fs, string path)
        {
            if (path.EndsWith(".obj")) return WavefrontObj.Load(fs, path);
            else if (path.EndsWith(".mdf")) return MeshDefFile.Load(fs, path).mapBones();
            else if (path.EndsWith(".3ds")) return load.Loader_3ds.Load(fs, path);
            else throw new UserException(path + " does not use a known fileformat");
        }
        public static void Save(Stream s, MeshDef def)
        {
            MeshDefFile.Write(s, def);
        }
        public static class MeshDefFile
        {
            internal static MeshDef Load(FileSystem fs, string path)
            {
                MeshDef def = new MeshDef();
                using( Stream s = fs.open(path))
                {
                    BinaryReader br = new BinaryReader(s);
                    int version = br.ReadInt32();
                    int materials = br.ReadInt32();
                    for (int materialid = 0; materialid < materials; ++materialid)
                    {
                        MeshDef.MaterialDef m = def.addMaterial("m" + materialid.ToString());
                        m.texture = br.ReadString();
                        m.ambient = vec3.Read(br);
                        m.diffuse = vec3.Read(br);
                        m.specular = vec3.Read(br);
                        m.emissive = vec3.Read(br);
                        m.alpha = br.ReadSingle();
                        m.shininess = br.ReadSingle();
                    }
                    int bonecount = br.ReadInt32();
                    for (int boneid = 0; boneid < bonecount; ++boneid)
                    {
                        MeshDef.Bone bone = def.newBone();
                        bone.name = br.ReadString();
                        bone.parent = br.ReadInt32();
                        bone.pos = vec3.Read(br);
                        vec3 qv = vec3.Read(br);
                        bone.rot = new quat(br.ReadSingle(), qv);
                    }
                    int pointcount = br.ReadInt32();
                    for(int pointid = 0; pointid < pointcount; ++pointid)
                    {
                        int boneid = br.ReadInt32();
                        vec3 p = new vec3(br.ReadSingle(), br.ReadSingle(), br.ReadSingle());
                        def.addPoint(p, boneid);
                    }
                    int uvcount = br.ReadInt32();
                    for (int uvid = 0; uvid < uvcount; ++uvid)
                    {
                        vec2 v = new vec2(br.ReadSingle(), br.ReadSingle());
                        def.AddUv(v);
                    }
                    int normalcount = br.ReadInt32();
                    for (int normalid = 0; normalid < normalcount; ++normalid)
                    {
                        vec3 p = new vec3(br.ReadSingle(), br.ReadSingle(), br.ReadSingle());
                        def.addNomal(p);
                    }

                    for (int materialid = 0; materialid < materials; ++materialid)
                    {
                        def.selectMaterial("m" + materialid.ToString());
                        int tricount = br.ReadInt32();
                        for (int triid = 0; triid < tricount; ++triid)
                        {
                            MeshDef.VertexData[] data = new MeshDef.VertexData[3];
                            for (int i = 0; i < 3; ++i)
                            {
                                data[i].vertex = br.ReadInt32();
                                data[i].uv = br.ReadInt32();
                                data[i].normal = br.ReadInt32();
                            }
                            def.addTri(new MeshDef.Tri(data));
                        }
                    }
                }
                return def;
            }

            

            internal static void Write(Stream s, MeshDef def)
            {
                BinaryWriter bw = new BinaryWriter(s);

                bw.Write((int)0);
                List<MeshDef.MaterialDef> materials = new List<MeshDef.MaterialDef>(def.Materials);
                bw.Write(materials.Count);
                foreach (MeshDef.MaterialDef mat in materials)
                {
                    bw.Write(mat.texture);
                    WriteColor(bw, mat.ambient);
                    WriteColor(bw, mat.diffuse);
                    WriteColor(bw, mat.specular);
                    WriteColor(bw, mat.emissive);
                    bw.Write(mat.alpha);
                    bw.Write(mat.shininess);
                }
                bw.Write(def.bones.Count);
                foreach (MeshDef.Bone bone in def.bones)
                {
                    bw.Write(bone.name);
                    bw.Write(bone.parent);
                    bw.Write(bone.pos.x);
                    bw.Write(bone.pos.y);
                    bw.Write(bone.pos.z);
                    bw.Write(bone.rot.x);
                    bw.Write(bone.rot.y);
                    bw.Write(bone.rot.z);
                    bw.Write(bone.rot.w);
                }
                bw.Write(def.points.Count);
                foreach (MeshDef.PointData v in def.points)
                {
                    bw.Write(v.boneid);
                    bw.Write(v.location.x);
                    bw.Write(v.location.y);
                    bw.Write(v.location.z);
                }
                bw.Write(def.uvs.Count);
                foreach(vec2 u in def.uvs)
                {
                    bw.Write(u.x);
                    bw.Write(u.y);
                }
                bw.Write(def.normals.Count);
                foreach(vec3 n in def.normals)
                {
                    bw.Write(n.x);
                    bw.Write(n.y);
                    bw.Write(n.z);
                }
                foreach (MeshDef.MaterialDef mat in materials)
                {
                    List<MeshDef.Tri> tris = new List<MeshDef.Tri>(def.TrianglesFor(mat));
                    bw.Write(tris.Count);
                    foreach (MeshDef.Tri tri in tris)
                    {
                        for (int i = 0; i < 3; ++i)
                        {
                            bw.Write(tri.data[i].vertex);
                            bw.Write(tri.data[i].uv);
                            bw.Write(tri.data[i].normal);
                        }
                    }
                }
            }

            private static void WriteColor(BinaryWriter bw, vec3 mat)
            {
                bw.Write(mat.x);
                bw.Write(mat.y);
                bw.Write(mat.z);
            }
        }
        public static class WavefrontObj
        {
            private static float floatParse(string s)
            {
                return float.Parse(s.Replace(".", ","));
            }
            public static MeshDef Load(FileSystem fs, string path)
            {
                MeshDef mesh = new MeshDef();
                using (var file = fs.open(path))
                {
                    foreach (string l in FileUtil.LinesIn(file))
                    {
                        string line = l.Trim();
                        if (string.IsNullOrEmpty(line) == false && line[0] != '#')
                        {
                            string[] data = line.Split(" \t".ToCharArray());
                            if (data[0] == "v")
                            {
                                mesh.addPoint(new vec3(floatParse(data[1]), floatParse(data[2]), floatParse(data[3])), -1);
                            }
                            else if (data[0] == "vt")
                            {
                                mesh.AddUv(new vec2(floatParse(data[1]), floatParse(data[2])));
                            }
                            else if (data[0] == "vn")
                            {
                                mesh.addNomal(new vec3(floatParse(data[1]), floatParse(data[2]), floatParse(data[3])));
                            }
                            else if (data[0] == "f")
                            {
                                List<MeshDef.VertexData> vd = new List<MeshDef.VertexData>();
                                for (int i = 1; i < data.Length; ++i)
                                {
                                    string[] ind = data[i].Split("/".ToCharArray());
                                    MeshDef.VertexData v = new MeshDef.VertexData();
                                    v.vertex = int.Parse(ind[0]) - 1;
                                    v.uv = int.Parse(ind[1]) - 1;
                                    v.normal = int.Parse(ind[2]) - 1;
                                    vd.Add(v);
                                }
                                if (vd.Count < 3) throw new Exception("Face data incomplete");
                                for (int i = 2; i < vd.Count; ++i)
                                {
                                    List<MeshDef.VertexData> arr = new List<MeshDef.VertexData>();
                                    arr.Add(vd[0]);
                                    arr.Add(vd[1]);
                                    arr.Add(vd[i]);
                                    mesh.addTri(new MeshDef.Tri(arr.ToArray()));
                                }
                            }
                            else if (data[0] == "usemtl")
                            {
                                mesh.selectMaterial(data[1].Trim());
                            }
                            else if (data[0] == "mtllib")
                            {
                                LoadMaterialLibrary(mesh, fs, Resolve(path, data[1].Trim()));
                            }
                        }
                    }
                }
                return mesh;
            }

            private static void LoadMaterialLibrary(MeshDef mesh, FileSystem fs, string path)
            {
                MeshDef.MaterialDef mat = null;
                using (var file = fs.open(path))
                {
                    foreach (string l in FileUtil.LinesIn(file))
                    {
                        string line = l.Trim();
                        if (string.IsNullOrEmpty(line) == false && line[0] != '#')
                        {
                            string[] data = line.Split(" \t".ToCharArray());
                            if (data[0] == "newmtl")
                            {
                                mat = mesh.addMaterial(data[1].Trim());
                            }
                            else if (data[0] == "Ka")
                            {
                                mat.ambient = ParseColor(data);
                            }
                            else if (data[0] == "Ke")
                            {
                                mat.emissive = ParseColor(data);
                            }
                            else if (data[0] == "Kd")
                            {
                                mat.diffuse = ParseColor(data);
                            }
                            else if (data[0] == "Ks")
                            {
                                mat.specular = ParseColor(data);
                            }
                            else if (data[0] == "d" || data[0] == "Tr")
                            {
                                mat.alpha = floatParse(data[1]);
                            }
                            else if (data[0] == "Ns")
                            {
                                mat.shininess = floatParse(data[1]);
                            }
                            else if (data[0] == "map_Ka" || data[0] == "map_Kd")
                            {
                                mat.texture = Resolve(path, data[1]);
                            }
                        }
                    }
                }
            }

            private static vec3 ParseColor(string[] data)
            {
                float r = floatParse(data[1]);
                float g = floatParse(data[1]);
                float b = floatParse(data[1]);
                return new vec3(r, g, b);
            }
        }

        // baseppath = hello:world.png
        // file = C:\foobar\user.bmp
        // should return hello:user.bmp
        private static string Resolve(string basepath, string file)
        {
            return Path.GetFileName(file);
        }
    }
}
