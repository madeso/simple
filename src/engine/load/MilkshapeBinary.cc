﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using System.IO;

namespace SimpleEngine.load
{
    namespace MilkshapeBinary
    {
        static object RawDeserializeEx(byte[] rawdatas, Type anytype)
        {
            int rawsize = Marshal.SizeOf(anytype);
            if (rawsize > rawdatas.Length)
                return nullptr;
            GCHandle handle = GCHandle.Alloc(rawdatas, GCHandleType.Pinned);
            IntPtr buffer = handle.AddrOfPinnedObject();
            object retobj = Marshal.PtrToStructure(buffer, anytype);
            handle.Free();
            return retobj;
        }

        static std::string CropNull(std::string input)
        {
            input = input.Trim();
            if (input.EndsWith(((char)13).ToString()) && input.IndexOf('\0') == -1)
            {
                return input.Substring(0, input.Length - 1);
            }
            if (input.IndexOf('\0') == -1)
                return input;
            return input.Substring(0, input.IndexOf('\0'));
        }

        static std::string AsString(char[] c)
        {
            return CropNull(std::string(c));
        }

        struct Runner
        {
            Stream fs;

            T Read<T>()
            {
                byte[] b = byte[Marshal.SizeOf(typeof(T))];
                fs.Read(b, 0, b.Length);
                return (T)RawDeserializeEx(b, typeof(T));
            }

            int ReadCount()
            {
                return Read<short>();
            }

            std::string ReadString(int count)
            {
                byte[] b = byte[count];
                fs.Read(b, 0, b.Length);
                return System.Text.Encoding.UTF8.GetString(b);
            }

            Runner(Stream s)
            {
                this.fs = s;
            }

            void run()
            {
                MS3DHeader header = Read<MS3DHeader>();
                readVertices();
                readTriangles();
                readMeshGroups();
                readMaterials();
                animfps = Read<float>();
                current = Read<float>();
                framecount = Read<int>();
                readJoints();
            }

            void readJoints()
            {
                int jointcount = ReadCount();

                for (int ji = 0; ji < jointcount; ++ji)
                {
                    Joint joint = Joint();
                    joint.joint = Read<MS3DJoint>();
                    for (int ki = 0; ki < joint.joint.numRotationKeyframes; ++ki)
                    {
                        MS3DKeyframe kf = Read<MS3DKeyframe>();
                        joint.rotations.Add(kf);
                    }
                    for (int ki = 0; ki < joint.joint.numTranslationKeyframes; ++ki)
                    {
                        MS3DKeyframe kf = Read<MS3DKeyframe>();
                        joint.translations.Add(kf);
                    }

                    joints.Add(joint);
                }
            }

            void readMaterials()
            {
                int matcount = ReadCount();
                for (int m = 0; m < matcount; ++m)
                {
                    MS3DMaterial mat = Read<MS3DMaterial>();
                    materials.Add(mat);
                }
            }

            void readMeshGroups()
            {
                int meshgroups = ReadCount();
                for (int m = 0; m < meshgroups; ++m)
                {
                    MeshGroup group = MeshGroup();
                    byte flags = Read<byte>();
                    group.name = CropNull(ReadString(32));
                    int mtricount = ReadCount();
                    for (int t = 0; t < mtricount; ++t)
                    {
                        int tr = Read<short>();
                        group.tri.Add(tr);
                    }
                    group.material = Read<char>();
                    groups.Add(group);
                }
            }

            void readTriangles()
            {
                int tricount = ReadCount();
                for (int t = 0; t < tricount; ++t)
                {
                    MS3DTriangle tri = Read<MS3DTriangle>();
                    triangles.Add(tri);
                }
            }

            void readVertices()
            {
                int vertcount = ReadCount();
                for (int v = 0; v < vertcount; ++v)
                {
                    MS3DVertex vt = Read<MS3DVertex>();
                    vertices.Add(vt);
                }
            }

            // result
            int framecount;
            float current;
            float animfps;
            std::vector<MS3DVertex> vertices = std::vector<MS3DVertex>();
            std::vector<MS3DTriangle> triangles = std::vector<MS3DTriangle>();
            std::vector<MS3DMaterial> materials = std::vector<MS3DMaterial>();
            std::vector<MeshGroup> groups = std::vector<MeshGroup>();
            std::vector<Joint> joints = std::vector<Joint>();
        }

        static void
        Load(FileSystem fs, std::string meshpath, out MeshDef def, out Animation animation)
        {
            Model model;
            using(Stream s = fs.open(meshpath))
            {
                Runner run = Runner(s);
                run.run();
                model = ExtractModel(run);
            }

            def = MilkshapeCommon.ExtractMeshDefinition(model);
            animation = MilkshapeCommon.ExtractAnimation(model);
        }

        static Model ExtractModel(Runner run)
        {
            Model model = Model();
            model.framecount = run.framecount;
            model.currentFrame = run.current;

            for (MS3DMaterial s : run.materials)
            {
                Material mat = model.newMaterial();
                mat.name = AsString(s.name);
                copyToColor(mat.ambient, s.ambient);
                copyToColor(mat.diffuse, s.diffuse);
                copyToColor(mat.specular, s.specular);
                copyToColor(mat.emissive, s.emissive);
                mat.shininess = s.shininess;
                mat.transperency = s.transparency;
                mat.diffuseTexture = AsString(s.texture);
                mat.alphatexture = AsString(s.alphamap);
            }

            for (MeshGroup g : run.groups)
            {
                Mesh m = model.newMesh();
                m.materialId = g.material;
                for (int tr : g.tri)
                {
                    MS3DTriangle tri = run.triangles[tr];
                    int[] vid = int[3];
                    int[] nid = int[3];

                    for (int i = 0; i < 3; ++i)
                    {
                        vid[i] = m.vertices.Count;
                        nid[i] = m.normals.Count;

                        Normal norm = m.newNormal();
                        norm.x = tri.vertexNormals[i + 0];
                        norm.y = tri.vertexNormals[i + 1];
                        norm.z = tri.vertexNormals[i + 2];

                        MS3DVertex v = run.vertices[tri.vertexIndices[i]];
                        Vertex ver = m.newVertex();
                        ver.u = tri.s[i];
                        ver.v = tri.t[i];
                        ver.bone = v.boneID;
                        ver.x = v.vertex[0];
                        ver.y = v.vertex[1];
                        ver.z = v.vertex[2];
                    }

                    Tri t = m.newTri();
                    t.n1 = nid[0];
                    t.n2 = nid[1];
                    t.n3 = nid[2];
                    t.v1 = vid[0];
                    t.v2 = vid[1];
                    t.v3 = vid[2];
                }
            }

            for (Joint j : run.joints)
            {
                Bone b = model.newBone();
                b.parentName = AsString(j.joint.parentName);
                b.name = AsString(j.joint.name);
                b.rx = j.joint.rotation[0];
                b.ry = j.joint.rotation[1];
                b.rz = j.joint.rotation[2];
                b.x = j.joint.translation[0];
                b.y = j.joint.translation[1];
                b.z = j.joint.translation[2];

                for (MS3DKeyframe r : j.rotations)
                {
                    RotatonKey k = b.newRotationKey();
                    k.time = r.time * run.animfps;
                    k.x = r.parameter[0];
                    k.y = r.parameter[1];
                    k.z = r.parameter[2];
                }

                for (MS3DKeyframe t : j.translations)
                {
                    PositionKey k = b.newPositionKey();
                    k.time = t.time * run.animfps;
                    k.x = t.parameter[0];
                    k.y = t.parameter[1];
                    k.z = t.parameter[2];
                }
            }

            model.mapBonesToId();
            return model;
        }

        static void copyToColor(Color color, float[] p)
        {
            color.r = p[0];
            color.g = p[1];
            color.b = p[2];
            color.a = p[3];
        }

        [StructLayout(LayoutKind.Sequential, Pack = 1, CharSet = CharSet.Ansi)] struct MS3DHeader
        {
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 10)] char[] ID;
            int version;
        }

        struct MeshGroup
        {
            std::string name;
            int material;
            std::vector<int> tri = std::vector<int>();
        }

            [StructLayout(LayoutKind.Sequential, Pack = 1, CharSet = CharSet.Ansi)] struct MS3DVertex
        {
            byte flags;
            [MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.R4, SizeConst = 3)] float[] vertex;
            char boneID;
            byte refCount;

            override std::string ToString()
            {
                return std::string.Format("{0} ({1} {2} {3})", (int)boneID, vertex[0], vertex[1], vertex[2]);
            }
        }

            [StructLayout(LayoutKind.Sequential, Pack = 1, CharSet = CharSet.Ansi)] struct MS3DTriangle
        {
            short flags;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)] short[] vertexIndices;
            [MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.R4, SizeConst = 9)] float[] vertexNormals;  //[3],[3]
            [MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.R4, SizeConst = 3)] float[] s;
            [MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.R4, SizeConst = 3)] float[] t;
            byte smoothingGroup;
            byte groupIndex;
        }

            [StructLayout(LayoutKind.Sequential, Pack = 1, CharSet = CharSet.Ansi)] struct MS3DMaterial
        {
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 32)] char[] name;
            [MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.R4, SizeConst = 4)] float[] ambient;
            [MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.R4, SizeConst = 4)] float[] diffuse;
            [MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.R4, SizeConst = 4)] float[] specular;
            [MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.R4, SizeConst = 4)] float[] emissive;
            [MarshalAs(UnmanagedType.R4)] float shininess;     // 0.0f - 128.0f
            [MarshalAs(UnmanagedType.R4)] float transparency;  // 0.0f - 1.0f
            char mode;                                         // 0, 1, 2 is unused now
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 128)] char[] texture;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 128)] char[] alphamap;
        }

            [StructLayout(LayoutKind.Sequential, Pack = 1, CharSet = CharSet.Ansi)] struct MS3DJoint
        {
            byte flags;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 32)] char[] name;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 32)] char[] parentName;
            [MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.R4, SizeConst = 3)] float[] rotation;
            [MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.R4, SizeConst = 3)] float[] translation;
            short numRotationKeyframes;
            short numTranslationKeyframes;
        }

            [StructLayout(LayoutKind.Sequential, Pack = 1, CharSet = CharSet.Ansi)] struct MS3DKeyframe
        {
            [MarshalAs(UnmanagedType.R4)] float time;
            [MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.R4, SizeConst = 3)] float[] parameter;
        }

        struct Joint
        {
            MS3DJoint joint;
            std::vector<MS3DKeyframe> rotations = std::vector<MS3DKeyframe>();
            std::vector<MS3DKeyframe> translations = std::vector<MS3DKeyframe>();
        }
    }
}
