using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace SimpleEngine
{
    namespace MeshFile
    {
        static MeshDef Load(FileSystem fs, std::string path)
        {
            if (path.EndsWith(".obj"))
                return WavefrontObj.Load(fs, path);
            else if (path.EndsWith(".mdf"))
                return MeshDefFile.Load(fs, path).mapBones();
            else if (path.EndsWith(".3ds"))
                return load.Loader_3ds.Load(fs, path);
            else
                throw UserException(path + " does not use a known fileformat");
        }
        static void Save(Stream s, MeshDef def)
        {
            MeshDefFile.Write(s, def);
        }
        namespace MeshDefFile
        {
            static MeshDef Load(FileSystem fs, std::string path)
            {
                MeshDef def = MeshDef();
                using(Stream s = fs.open(path))
                {
                    BinaryReader br = BinaryReader(s);
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
                        bone.rot = quat(br.ReadSingle(), qv);
                    }
                    int pointcount = br.ReadInt32();
                    for (int pointid = 0; pointid < pointcount; ++pointid)
                    {
                        int boneid = br.ReadInt32();
                        vec3 p = vec3(br.ReadSingle(), br.ReadSingle(), br.ReadSingle());
                        def.addPoint(p, boneid);
                    }
                    int uvcount = br.ReadInt32();
                    for (int uvid = 0; uvid < uvcount; ++uvid)
                    {
                        vec2 v = vec2(br.ReadSingle(), br.ReadSingle());
                        def.AddUv(v);
                    }
                    int normalcount = br.ReadInt32();
                    for (int normalid = 0; normalid < normalcount; ++normalid)
                    {
                        vec3 p = vec3(br.ReadSingle(), br.ReadSingle(), br.ReadSingle());
                        def.addNomal(p);
                    }

                    for (int materialid = 0; materialid < materials; ++materialid)
                    {
                        def.selectMaterial("m" + materialid.ToString());
                        int tricount = br.ReadInt32();
                        for (int triid = 0; triid < tricount; ++triid)
                        {
                            MeshDef.VertexData[] data = MeshDef.VertexData[3];
                            for (int i = 0; i < 3; ++i)
                            {
                                data[i].vertex = br.ReadInt32();
                                data[i].uv = br.ReadInt32();
                                data[i].normal = br.ReadInt32();
                            }
                            def.addTri(MeshDef.Tri(data));
                        }
                    }
                }
                return def;
            }

            static void Write(Stream s, MeshDef def)
            {
                BinaryWriter bw = BinaryWriter(s);

                bw.Write((int)0);
                std::vector<MeshDef.MaterialDef> materials = std::vector<MeshDef.MaterialDef>(def.Materials);
                bw.Write(materials.Count);
                for (MeshDef.MaterialDef mat : materials)
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
                for (MeshDef.Bone bone : def.bones)
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
                for (MeshDef.PointData v : def.points)
                {
                    bw.Write(v.boneid);
                    bw.Write(v.location.x);
                    bw.Write(v.location.y);
                    bw.Write(v.location.z);
                }
                bw.Write(def.uvs.Count);
                for (vec2 u : def.uvs)
                {
                    bw.Write(u.x);
                    bw.Write(u.y);
                }
                bw.Write(def.normals.Count);
                for (vec3 n : def.normals)
                {
                    bw.Write(n.x);
                    bw.Write(n.y);
                    bw.Write(n.z);
                }
                for (MeshDef.MaterialDef mat : materials)
                {
                    std::vector<MeshDef.Tri> tris = std::vector<MeshDef.Tri>(def.TrianglesFor(mat));
                    bw.Write(tris.Count);
                    for (MeshDef.Tri tri : tris)
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

            static void WriteColor(BinaryWriter bw, vec3 mat)
            {
                bw.Write(mat.x);
                bw.Write(mat.y);
                bw.Write(mat.z);
            }
        }
        namespace WavefrontObj
        {
            static float floatParse(std::string s)
            {
                return float.Parse(s.Replace(".", ","));
            }
            static MeshDef Load(FileSystem fs, std::string path)
            {
                MeshDef mesh = MeshDef();
                using(var file = fs.open(path))
                {
                    for (std::string l : FileUtil.LinesIn(file))
                    {
                        std::string line = l.Trim();
                        if (std::string.IsNullOrEmpty(line) == false && line[0] != '#')
                        {
                            std::string[] data = line.Split(" \t".ToCharArray());
                            if (data[0] == "v")
                            {
                                mesh.addPoint(vec3(floatParse(data[1]), floatParse(data[2]), floatParse(data[3])), -1);
                            }
                            else if (data[0] == "vt")
                            {
                                mesh.AddUv(vec2(floatParse(data[1]), floatParse(data[2])));
                            }
                            else if (data[0] == "vn")
                            {
                                mesh.addNomal(vec3(floatParse(data[1]), floatParse(data[2]), floatParse(data[3])));
                            }
                            else if (data[0] == "f")
                            {
                                std::vector<MeshDef.VertexData> vd = std::vector<MeshDef.VertexData>();
                                for (int i = 1; i < data.Length; ++i)
                                {
                                    std::string[] ind = data[i].Split("/".ToCharArray());
                                    MeshDef.VertexData v = MeshDef.VertexData();
                                    v.vertex = int.Parse(ind[0]) - 1;
                                    v.uv = int.Parse(ind[1]) - 1;
                                    v.normal = int.Parse(ind[2]) - 1;
                                    vd.Add(v);
                                }
                                if (vd.Count < 3)
                                    throw Exception("Face data incomplete");
                                for (int i = 2; i < vd.Count; ++i)
                                {
                                    std::vector<MeshDef.VertexData> arr = std::vector<MeshDef.VertexData>();
                                    arr.Add(vd[0]);
                                    arr.Add(vd[1]);
                                    arr.Add(vd[i]);
                                    mesh.addTri(MeshDef.Tri(arr.ToArray()));
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

            static void LoadMaterialLibrary(MeshDef mesh, FileSystem fs, std::string path)
            {
                MeshDef.MaterialDef mat = nullptr;
                using(var file = fs.open(path))
                {
                    for (std::string l : FileUtil.LinesIn(file))
                    {
                        std::string line = l.Trim();
                        if (std::string.IsNullOrEmpty(line) == false && line[0] != '#')
                        {
                            std::string[] data = line.Split(" \t".ToCharArray());
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

            static vec3 ParseColor(std::string[] data)
            {
                float r = floatParse(data[1]);
                float g = floatParse(data[1]);
                float b = floatParse(data[1]);
                return vec3(r, g, b);
            }
        }

        // baseppath = hello:world.png
        // file = C:\foobar\user.bmp
        // should return hello:user.bmp
        static std::string
        Resolve(std::string basepath, std::string file)
        {
            return Path.GetFileName(file);
        }
    }
}
