using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimpleEngine.load
{
    struct Model
    {
        int framecount = 0;
        float currentFrame = 0;

        std::vector<Mesh> meshes = std::vector<Mesh>();

        Mesh newMesh()
        {
            Mesh m = Mesh();
            meshes.Add(m);
            return m;
        }

        std::vector<Material> materials = std::vector<Material>();
        std::vector<Bone> bones = std::vector<Bone>();

        Material newMaterial()
        {
            Material mat = Material();
            materials.Add(mat);
            return mat;
        }

        Bone newBone()
        {
            Bone b = Bone();
            bones.Add(b);
            return b;
        }

        int boneId(std::string bone)
        {
            if (bone == "")
                return -1;
            for (int i = 0; i < bones.Count; ++i)
            {
                if (bones[i].name == bone)
                    return i;
            }
            throw std::runtime_error("Failed to find a match for " + bone);
        }

        void mapBonesToId()
        {
            for (Bone b : bones)
            {
                b.parentId = boneId(b.parentName);
            }
        }
    } struct Mesh
    {
        std::string name = "";
        int flags = 0;
        int materialId = 0;

        std::vector<Vertex> vertices = std::vector<Vertex>();
        std::vector<Normal> normals = std::vector<Normal>();
        std::vector<Tri> tris = std::vector<Tri>();

        Vertex newVertex()
        {
            Vertex v = Vertex();
            vertices.Add(v);
            return v;
        }

        Normal newNormal()
        {
            Normal n = Normal();
            normals.Add(n);
            return n;
        }

        Tri newTri()
        {
            Tri t = Tri();
            tris.Add(t);
            return t;
        }
    } struct Vertex
    {
        int flags = 0;
        float x
        {
            set { pos.x = value; }
            get { return pos.x; }
        }
        float y
        {
            set { pos.y = value; }
            get { return pos.y; }
        }
        float z
        {
            set { pos.z = value; }
            get { return pos.z; }
        }
        float u
        {
            set { uv.x = value; }
            get { return uv.x; }
        }
        float v
        {
            set { uv.y = value; }
            get { return uv.y; }
        }

        override std::string ToString()
        {
            return std::string.Format("{0} {1} / {2}", bone, pos, uv);
        }

        vec2 uv = vec2(0, 0);
        vec3 pos = vec3(0, 0, 0);
        int bone = 0;
    } struct Normal
    {
        float x
        {
            get { return norm.x; }
            set { norm.x = value; }
        }
        float y
        {
            get { return norm.y; }
            set { norm.y = value; }
        }
        float z
        {
            get { return norm.z; }
            set { norm.z = value; }
        }

        vec3 norm = vec3(0, 0, 0);

        void normalize()
        {
            norm.normalize();
        }
    } struct Tri
    {
        int flags = 0;
        int v1 = 0;
        int v2 = 0;
        int v3 = 0;
        int n1 = 0;
        int n2 = 0;
        int n3 = 0;
        int smoothingGroup = 0;

        void buildNormal(Mesh mesh)
        {
            // not needed?
        }
    } struct Color
    {
        float r = 1;
        float g = 1;
        float b = 1;
        float a = 1;

        void parse(std::string p)
        {
            std::string[] data = p.Split(" ".ToCharArray());
            r = math1::ParseFloat(data[0]);
            g = math1::ParseFloat(data[1]);
            b = math1::ParseFloat(data[2]);
            a = math1::ParseFloat(data[3]);
        }
    } struct Material
    {
        std::string name = "";
        Color ambient = Color();
        Color diffuse = Color();
        Color specular = Color();
        Color emissive = Color();
        float shininess = 0;
        float transperency = 1;
        std::string diffuseTexture = "";
        std::string alphatexture = "";
    } struct PositionKey
    {
        float time;
        float x;
        float y;
        float z;
    } struct RotatonKey
    {
        float time;
        float x;
        float y;
        float z;

        override std::string ToString()
        {
            return std::string.Format("{0} {1}", time, MilkshapeCommon.makeQuat(vec3(x, y, z)).AxisAngle);
        }
    } struct Bone
    {
        std::string name = "";
        std::string parentName = "";
        int parentId = -1;
        int flags;
        float x;
        float y;
        float z;
        float rx;
        float ry;
        float rz;

        override std::string ToString()
        {
            return name + ": " + parentName;
        }

        std::vector<PositionKey> positions = std::vector<PositionKey>();
        std::vector<RotatonKey> rotations = std::vector<RotatonKey>();

        IEnumerable<PositionKey> PositionKeys
        {
            get
            {
                return positions;
            }
        }

        IEnumerable<RotatonKey> RotationKeys
        {
            get
            {
                return rotations;
            }
        }

        RotatonKey newRotationKey()
        {
            RotatonKey key = RotatonKey();
            rotations.Add(key);
            return key;
        }

        PositionKey newPositionKey()
        {
            PositionKey key = PositionKey();
            positions.Add(key);
            return key;
        }
    }

    namespace MilkshapeCommon
    {
        static Animation ExtractAnimation(Model model)
        {
            std::vector<AnimationForBone> ani = std::vector<AnimationForBone>();

            bool added = false;

            for (Bone b : model.bones)
            {
                AnimationForBone a = AnimationForBone();

                for (PositionKey pk : b.PositionKeys)
                {
                    a.addPositon(pk.time, vec3(pk.x, pk.y, pk.z));
                    added = true;
                }
                for (RotatonKey rk : b.RotationKeys)
                {
                    a.addRotation(rk.time, makeQuat(vec3(rk.x, rk.y, rk.z)));
                    added = true;
                }

                ani.Add(a);
            }

            if (added)
                return Animation(ani);
            else
                return nullptr;
        }
        static MeshDef ExtractMeshDefinition(Model model)
        {
            MeshDef def = MeshDef();
            int id = 0;
            for (Material mat : model.materials)
            {
                MeshDef.MaterialDef smat = def.addMaterial("m" + id.ToString());
                smat.texture = SmartTexture(mat.diffuseTexture);
                ++id;
            }

            for (Bone b : model.bones)
            {
                MeshDef.Bone bn = def.newBone();
                bn.pos = vec3(b.x, b.y, b.z);
                bn.rot = makeQuat(vec3(b.rx, b.ry, b.rz));
                bn.parent = b.parentId;
                bn.name = b.name;
            }

            int vadded = 0;
            int vbase = 0;
            int nadded = 0;
            int nbase = 0;
            for (Mesh me : model.meshes)
            {
                vbase += vadded;
                vadded = 0;
                nbase += nadded;
                nadded = 0;

                def.selectMaterial("m" + me.materialId);
                for (Vertex v : me.vertices)
                {
                    def.addPoint(v.pos, v.bone);
                    def.AddUv(vec2(v.u, 1 - v.v));
                    ++vadded;
                }

                for (Normal n : me.normals)
                {
                    def.addNomal(n.norm);
                    ++nadded;
                }

                for (Tri tr : me.tris)
                {
                    MeshDef.VertexData[] data = MeshDef.VertexData[3];
                    data[0].uv = data[0].vertex = vbase + tr.v1;
                    data[1].uv = data[1].vertex = vbase + tr.v2;
                    data[2].uv = data[2].vertex = vbase + tr.v3;
                    data[0].normal = nbase + tr.n1;
                    data[1].normal = nbase + tr.n2;
                    data[2].normal = nbase + tr.n3;
                    def.addTri(MeshDef.Tri(data));
                }
            }

            return def;
        }
        static float sin(float s)
        {
            return (float)Math.Sin(s);
        }
        static float cos(float s)
        {
            return (float)Math.Cos(s);
        }
        static quat makeQuat(vec3 angles)
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

            float x = sr * cp * cy - cr * sp * sy;  // X
            float y = cr * sp * cy + sr * cp * sy;  // Y
            float z = cr * cp * sy - sr * sp * cy;  // Z
            float w = cr * cp * cy + sr * sp * sy;  // W
            quat a = quat(w, vec3(x, y, z)).Normalized;
            return a;
        }
        static std::string SmartTexture(std::string p)
        {
            return System.IO.Path.GetFileName(p);
        }
    }
}
