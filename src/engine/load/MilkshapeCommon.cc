using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimpleEngine.load
{
    internal class Model
    {
        public int framecount = 0;
        public float currentFrame = 0;

        public List<Mesh> meshes = new List<Mesh>();

        public Mesh newMesh()
        {
            Mesh m = new Mesh();
            meshes.Add(m);
            return m;
        }

        public List<Material> materials = new List<Material>();
        public List<Bone> bones = new List<Bone>();

        internal Material newMaterial()
        {
            Material mat = new Material();
            materials.Add(mat);
            return mat;
        }

        internal Bone newBone()
        {
            Bone b = new Bone();
            bones.Add(b);
            return b;
        }

        int boneId(string bone)
        {
            if (bone == "") return -1;
            for (int i = 0; i < bones.Count; ++i)
            {
                if (bones[i].name == bone) return i;
            }
            throw new Exception("Failed to find a match for " + bone);
        }

        public void mapBonesToId()
        {
            foreach (Bone b in bones)
            {
                b.parentId = boneId(b.parentName);
            }
        }
    }
    internal class Mesh
    {
        public string name = "";
        public int flags = 0;
        public int materialId = 0;

        public List<Vertex> vertices = new List<Vertex>();
        public List<Normal> normals = new List<Normal>();
        public List<Tri> tris = new List<Tri>();

        internal Vertex newVertex()
        {
            Vertex v = new Vertex();
            vertices.Add(v);
            return v;
        }

        internal Normal newNormal()
        {
            Normal n = new Normal();
            normals.Add(n);
            return n;
        }

        internal Tri newTri()
        {
            Tri t = new Tri();
            tris.Add(t);
            return t;
        }
    }
    internal class Vertex
    {
        public int flags = 0;
        public float x
        {
            set { pos.x = value; }
            get { return pos.x; }
        }
        public float y
        {
            set { pos.y = value; }
            get { return pos.y; }
        }
        public float z
        {
            set { pos.z = value; }
            get { return pos.z; }
        }
        public float u
        {
            set { uv.x = value; }
            get { return uv.x; }
        }
        public float v
        {
            set { uv.y = value; }
            get { return uv.y; }
        }

        public override string ToString()
        {
            return string.Format("{0} {1} / {2}",bone, pos, uv);
        }

        public vec2 uv = new vec2(0, 0);
        public vec3 pos = new vec3(0, 0, 0);
        public int bone = 0;
    }
    internal class Normal
    {
        public float x
        {
            get { return norm.x; }
            set { norm.x = value; }
        }
        public float y
        {
            get { return norm.y; }
            set { norm.y = value; }
        }
        public float z
        {
            get { return norm.z; }
            set { norm.z = value; }
        }

        public vec3 norm = new vec3(0, 0, 0);

        internal void normalize()
        {
            norm.normalize();
        }
    }
    internal class Tri
    {
        public int flags = 0;
        public int v1 = 0;
        public int v2 = 0;
        public int v3 = 0;
        public int n1 = 0;
        public int n2 = 0;
        public int n3 = 0;
        public int smoothingGroup = 0;

        internal void buildNormal(Mesh mesh)
        {
            // not needed?
        }
    }
    internal class Color
    {
        public float r = 1;
        public float g = 1;
        public float b = 1;
        public float a = 1;

        internal void parse(string p)
        {
            string[] data = p.Split(" ".ToCharArray());
            r = math1.ParseFloat(data[0]);
            g = math1.ParseFloat(data[1]);
            b = math1.ParseFloat(data[2]);
            a = math1.ParseFloat(data[3]);
        }
    }
    internal class Material
    {
        public string name = "";
        public Color ambient = new Color();
        public Color diffuse = new Color();
        public Color specular = new Color();
        public Color emissive = new Color();
        public float shininess = 0;
        public float transperency = 1;
        public string diffuseTexture = "";
        public string alphatexture = "";
    }
    internal class PositionKey
    {
        public float time;
        public float x;
        public float y;
        public float z;
    }
    internal class RotatonKey
    {
        public float time;
        public float x;
        public float y;
        public float z;

        public override string ToString()
        {
            return string.Format("{0} {1}", time, MilkshapeCommon.makeQuat(new vec3(x, y, z)).AxisAngle);
        }
    }
    internal class Bone
    {
        public string name = "";
        public string parentName = "";
        public int parentId = -1;
        public int flags;
        public float x;
        public float y;
        public float z;
        public float rx;
        public float ry;
        public float rz;

        public override string ToString()
        {
            return name + ": " + parentName;
        }

        List<PositionKey> positions = new List<PositionKey>();
        List<RotatonKey> rotations = new List<RotatonKey>();

        public IEnumerable<PositionKey> PositionKeys
        {
            get
            {
                return positions;
            }
        }

        public IEnumerable<RotatonKey> RotationKeys
        {
            get
            {
                return rotations;
            }
        }

        internal RotatonKey newRotationKey()
        {
            RotatonKey key = new RotatonKey();
            rotations.Add(key);
            return key;
        }

        internal PositionKey newPositionKey()
        {
            PositionKey key = new PositionKey();
            positions.Add(key);
            return key;
        }
    }

    internal static class MilkshapeCommon
    {
        internal static Animation ExtractAnimation(Model model)
        {
            List<AnimationForBone> ani = new List<AnimationForBone>();

            bool added = false;

            foreach (Bone b in model.bones)
            {
                AnimationForBone a = new AnimationForBone();

                foreach (PositionKey pk in b.PositionKeys)
                {
                    a.addPositon(pk.time, new vec3(pk.x, pk.y, pk.z));
                    added = true;
                }
                foreach (RotatonKey rk in b.RotationKeys)
                {
                    a.addRotation(rk.time, makeQuat(new vec3(rk.x, rk.y, rk.z)));
                    added = true;
                }

                ani.Add(a);
            }

            if (added) return new Animation(ani);
            else return null;
        }
        internal static MeshDef ExtractMeshDefinition(Model model)
        {
            MeshDef def = new MeshDef();
            int id = 0;
            foreach (Material mat in model.materials)
            {
                MeshDef.MaterialDef smat = def.addMaterial("m" + id.ToString());
                smat.texture = SmartTexture(mat.diffuseTexture);
                ++id;
            }

            foreach (Bone b in model.bones)
            {
                MeshDef.Bone bn = def.newBone();
                bn.pos = new vec3(b.x, b.y, b.z);
                bn.rot = makeQuat(new vec3(b.rx, b.ry, b.rz));
                bn.parent = b.parentId;
                bn.name = b.name;
            }

            int vadded = 0;
            int vbase = 0;
            int nadded = 0;
            int nbase = 0;
            foreach (Mesh me in model.meshes)
            {
                vbase += vadded;
                vadded = 0;
                nbase += nadded;
                nadded = 0;

                def.selectMaterial("m" + me.materialId);
                foreach (Vertex v in me.vertices)
                {
                    def.addPoint(v.pos, v.bone);
                    def.AddUv(new vec2(v.u, 1 - v.v));
                    ++vadded;
                }

                foreach (Normal n in me.normals)
                {
                    def.addNomal(n.norm);
                    ++nadded;
                }

                foreach (Tri tr in me.tris)
                {
                    MeshDef.VertexData[] data = new MeshDef.VertexData[3];
                    data[0].uv = data[0].vertex = vbase + tr.v1;
                    data[1].uv = data[1].vertex = vbase + tr.v2;
                    data[2].uv = data[2].vertex = vbase + tr.v3;
                    data[0].normal = nbase + tr.n1;
                    data[1].normal = nbase + tr.n2;
                    data[2].normal = nbase + tr.n3;
                    def.addTri(new MeshDef.Tri(data));
                }
            }

            return def;
        }
        internal static float sin(float s)
        {
            return (float)Math.Sin(s);
        }
        internal static float cos(float s)
        {
            return (float)Math.Cos(s);
        }
        internal static quat makeQuat(vec3 angles)
        {
            float ang;
            float sr, sp, sy, cr, cp, cy;

            // FIXME: rescale the inputs to 1/2 angle
            float ascale = 0.5f;
            ang = angles[2] * ascale;
            sy = sin(ang);
            cy = cos(ang);
            ang = angles[1] * ascale;
            sp = sin(ang);
            cp = cos(ang);
            ang = angles[0] * ascale;
            sr = sin(ang);
            cr = cos(ang);

            float x = sr * cp * cy - cr * sp * sy; // X
            float y = cr * sp * cy + sr * cp * sy; // Y
            float z = cr * cp * sy - sr * sp * cy; // Z
            float w = cr * cp * cy + sr * sp * sy; // W
            quat a = new quat(w, new vec3(x, y, z)).Normalized;
            return a;
        }
        internal static string SmartTexture(string p)
        {
            return System.IO.Path.GetFileName(p);
        }
    }
}
