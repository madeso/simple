using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimpleEngine
{
    public class MeshDef
    {
        public class PointData
        {
            public PointData(int bone, vec3 loc)
            {
                this.boneid = bone;
                this.location = loc;
            }
            public int boneid;
            public vec3 location;

            public override string ToString()
            {
                return string.Format("{0} {1}", boneid, location);
            }
        }

        public List<PointData> points = new List<PointData>();
        public List<vec2> uvs = new List<vec2>();
        public List<vec3> normals = new List<vec3>();
        public List<Bone> bones = new List<Bone>();

        public int TriCount
        {
            get
            {
                int c = 0;
                foreach (KeyValuePair<string, Data> d in datas)
                {
                    c += d.Value.tris.Count;
                }
                return c;
            }
        }

        public MeshDef mapBones()
        {
            Dictionary<int, KeyValuePair<mat44, List<PointData>>> bdp = new Dictionary<int, KeyValuePair<mat44, List<PointData>>>();

            for (int i = 0; i < bones.Count; ++i)
            {
                int parent = bones[i].parent;
                bones[i].index = i;
                if (bones[i].hasParent)
                {
                    bones[i].parentBone = bones[parent];
                    bones[i].parentBone.childs.Add(bones[i]);
                }

                Bone bone = bones[i];
                mat44 m = new MatrixHelper(mat44.Identity).Translate(-bone.pos).Rotate(-bone.rot).mat44;
                bdp.Add(i, new KeyValuePair<mat44, List<PointData>>(m, new List<PointData>()));
            }

            foreach (PointData pd in points)
            {
                if (pd.boneid == -1) continue;
                Bone bone = bones[pd.boneid];
                bdp[pd.boneid].Value.Add(pd);
            }

            foreach(KeyValuePair<int, KeyValuePair<mat44, List<PointData>>> k in bdp )
            {
                Bone bone = bones[k.Key];
                mat44 m = mat44.Identity;
                for(Bone b = bone; b != null; b = b.parentBone)
                {
                    m *= bdp[b.index].Key;
                }

                foreach(PointData pd in k.Value.Value )
                {
                    pd.location = m * pd.location;
                }
            }
            

            return this;
        }

        public IEnumerable<Bone> RootBones
        {
            get
            {
                foreach (Bone b in bones)
                {
                    if (b.parentBone == null)
                        yield return b;
                }
            }
        }

        public class Data
        {
            public List<Tri> tris = new List<Tri>();
        }
        Dictionary<string, Data> datas = new Dictionary<string, Data>();
        Data currentd = null;

        public class MaterialDef 
        {
            public MaterialDef(string name)
            {
                this.name = name;
            }
            public readonly string name;
            public vec3 ambient = new vec3(0.2f, 0.2f, 0.2f);
            public vec3 diffuse = new vec3(0.8f, 0.8f, 0.8f);
            public vec3 specular = new vec3(1,1,1);
            public vec3 emissive = new vec3(0,0,0);

            public float alpha = 1;
            public float shininess = 0;

            public string texture = "";

            public string TextureOrName
            {
                get
                {
                    if (texture == "") return name;
                    else return texture;
                }
            }

            public override string ToString()
            {
                return name + ": " + texture;
            }
        }

        public IEnumerable<Tri> TrianglesFor(MaterialDef material)
        {
            if (datas.ContainsKey(material.name))
            {
                foreach (Tri t in datas[material.name].tris)
                {
                    yield return t;
                }
            }
        }

        Dictionary<string, MaterialDef> materials = new Dictionary<string, MaterialDef>();

        public IEnumerable<MaterialDef> Materials
        {
            get
            {
                foreach (KeyValuePair<string, MaterialDef> k in materials)
                {
                    yield return k.Value;
                }
            }
        }

        public struct VertexData
        {
            public int vertex;
            public int normal;
            public int uv;
        }

        public struct Vertex
        {
            public vec3 vertex;
            public vec3 normal;
            public vec2 uv;
            public int bone;
        }

        public class Bone
        {
            public int parent;
            public string name;
            public vec3 pos;
            public quat rot;

            public int index = 0;
            public Bone parentBone = null;

            public override string ToString()
            {
                if (parentBone == null)
                {
                    return name;
                }
                else return parentBone.ToString() + "->" + name;
            }

            public bool hasParent
            {
                get
                {
                    return parent != -1;
                }
            }

            public List<Bone> childs = new List<Bone>();
        }

        public Vertex[] lookup(Tri tri)
        {
            List<Vertex> v = new List<Vertex>();
            for (int i = 0; i < 3; ++i)
            {
                Vertex c = new Vertex();
                c.vertex = points[tri.data[i].vertex].location;
                c.bone = points[tri.data[i].vertex].boneid;

                if (tri.data[i].normal != -1)
                {
                    c.normal = normals[tri.data[i].normal];
                }

                c.uv = uvs[tri.data[i].uv];
                v.Add(c);
            }
            return v.ToArray();
        }

        public class Tri
        {
            public Tri(VertexData[] data)
            {
                System.Diagnostics.Debug.Assert(data.Length == 3, "It is a triangle");
                this.data = data;
            }
            public VertexData[] data;
        }

        public void addPoint(vec3 p, int bone)
        {
            points.Add( new PointData(bone, p) );
        }

        public void AddUv(vec2 u)
        {
            uvs.Add(u);
        }

        public MaterialDef addMaterial(string name)
        {
            MaterialDef mat = new MaterialDef(name);
            materials.Add(name, mat);
            return mat;
        }

        public void selectMaterial(string name)
        {
            if (materials.ContainsKey(name)==false)
            {
                throw new Exception("mesh does not contain a material named " + name);
            }
            if (datas.ContainsKey(name)==false)
            {
                datas.Add(name, new Data());
            }
            currentd = datas[name];
        }

        public void addTri(Tri t)
        {
            currentd.tris.Add(t);
        }

        public void addNomal(vec3 v)
        {
            normals.Add(v);
        }

        private CompiledMesh compiledMesh = null;

        public CompiledMesh Compiled
        {
            get
            {
                if (compiledMesh == null) throw new Exception("mesh is not compiled");
                return compiledMesh;
            }
        }

        public void compile(MediaLoader ml)
        {
            compiledMesh = new CompiledMesh(ml, this);
        }

        public Bone newBone()
        {
            Bone b = new Bone();
            bones.Add(b);
            return b;
        }

        public bool hasTrianglesFor(MaterialDef m)
        {
            List<Tri> t = new List<Tri>(TrianglesFor(m));
            return t.Count != 0;
        }

        internal void scale(float scale)
        {
            foreach (PointData pd in points)
            {
                pd.location = pd.location * scale;
            }

            foreach (Bone b in bones)
            {
                b.pos = b.pos * scale;
            }
        }

        public MeshDef.MaterialDef getMaterialNamed(string name)
        {
            return materials[name];
        }

        internal void translateFiles(Dictionary<string, string> overrides)
        {
            foreach (MaterialDef d in Materials)
            {
                d.texture = FileSystem.MapFile(overrides, d.TextureOrName);
            }
        }
    }
}
