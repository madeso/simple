#include <array>
#include <cassert>
#include <map>
#include <memory>
#include <stdexcept>
#include <vector>

#include "engine/mat44.h"
#include "engine/matrixhelper.h"
#include "engine/quat.h"
#include "engine/vec2.h"
#include "engine/vec3.h"
#include "fmt/core.h"

namespace SimpleEngine
{
    struct CompiledMesh;
    struct MediaLoader;

    struct PointData
    {
        int boneid;
        vec3 location;

        PointData(int bone, const vec3& loc)
            : boneid(bone)
            , location(loc)
        {
        }

        std::string ToString() const
        {
            return fmt::format("{0} {1}", boneid, location.ToString());
        }
    };

    struct VertexData
    {
        int vertex;
        int normal;
        int uv;
    };

    using Tri = std::array<VertexData, 3>;
    struct Data
    {
        std::vector<Tri> tris = std::vector<Tri>();
    };

    struct Vertex
    {
        vec3 vertex;
        vec3 normal;
        vec2 uv;
        int bone;
    };

    struct Bone
    {
        int parent;
        std::string name;
        vec3 pos;
        quat rot;

        int index = 0;
        std::shared_ptr<Bone> parentBone;

        std::string ToString() const
        {
            if (parentBone == nullptr)
            {
                return name;
            }
            else
                return parentBone->ToString() + "->" + name;
        }

        bool hasParent() const
        {
            return parent != -1;
        }

        std::vector<std::shared_ptr<Bone>> childs;
    };

    struct MaterialDef
    {
        MaterialDef(const std::string& n)
            : name(n)
        {
        }

        std::string name;
        vec3 ambient = vec3(0.2f, 0.2f, 0.2f);
        vec3 diffuse = vec3(0.8f, 0.8f, 0.8f);
        vec3 specular = vec3(1, 1, 1);
        vec3 emissive = vec3(0, 0, 0);

        float alpha = 1;
        float shininess = 0;

        std::string texture;

        std::string TextureOrName() const
        {
            if (texture == "")
                return name;
            else
                return texture;
        }

        std::string ToString() const
        {
            return name + ": " + texture;
        }
    };

    struct Mpd
    {
        Mpd()
            : m(mat44::Identity())
        {
        }

        Mpd(const mat44& mm)
            : m(mm)
        {
        }

        mat44 m;
        std::vector<PointData> pd;
    };

    struct MeshDef
    {
        std::vector<PointData> points;
        std::vector<vec2> uvs;
        std::vector<vec3> normals;
        std::vector<std::shared_ptr<Bone>> bones;

        std::map<std::string, std::shared_ptr<Data>> datas;
        std::shared_ptr<Data> currentd;

        std::map<std::string, std::shared_ptr<MaterialDef>> materials;

        std::shared_ptr<CompiledMesh> compiledMesh;

        int TriCount() const
        {
            int c = 0;
            for (const auto& d : datas)
            {
                c += d.second->tris.size();
            }
            return c;
        }

        MeshDef& mapBones()
        {
            std::map<int, Mpd> bdp;

            for (int i = 0; i < bones.size(); ++i)
            {
                int parent = bones[i]->parent;
                bones[i]->index = i;
                if (bones[i]->hasParent())
                {
                    bones[i]->parentBone = bones[parent];
                    bones[i]->parentBone->childs.emplace_back(bones[i]);
                }

                auto& bone = bones[i];
                mat44 m = MatrixHelper(mat44::Identity()).Translate(-bone->pos).Rotate(-bone->rot).mat44();
                bdp.emplace(i, Mpd{m});
            }

            for (auto& pd : points)
            {
                if (pd.boneid == -1)
                    continue;
                const auto& bone = bones[pd.boneid];
                Mpd& mpd = bdp[pd.boneid];
                mpd.pd.emplace_back(pd);
            }

            for (auto& k : bdp)
            {
                auto& bone = bones[k.first];
                mat44 m = mat44::Identity();
                for (auto& b = bone; b != nullptr; b = b->parentBone)
                {
                    m = m * bdp[b->index].m;
                }

                for (PointData& pd : k.second.pd)
                {
                    pd.location = m * pd.location;
                }
            }

            return *this;
        }

        std::vector<std::shared_ptr<Bone>> RootBones()
        {
            std::vector<std::shared_ptr<Bone>> r;

            for (auto& b : bones)
            {
                if (b->parentBone == nullptr)
                {
                    r.emplace_back(b);
                }
            }

            return r;
        }

        std::vector<Tri> TrianglesFor(const MaterialDef& material) const
        {
            std::vector<Tri> r;

            if (auto found = datas.find(material.name); found != datas.end())
            {
                for (Tri t : found->second->tris)
                {
                    r.emplace_back(t);
                }
            }

            return r;
        }

        std::vector<std::shared_ptr<MaterialDef>> Materials()
        {
            std::vector<std::shared_ptr<MaterialDef>> r;
            for (auto& k : materials)
            {
                r.emplace_back(k.second);
            }
            return r;
        }

        std::vector<Vertex> lookup(const Tri& tri) const
        {
            std::vector<Vertex> v;
            for (int i = 0; i < 3; ++i)
            {
                Vertex c;
                c.vertex = points[tri[i].vertex].location;
                c.bone = points[tri[i].vertex].boneid;

                if (tri[i].normal != -1)
                {
                    c.normal = normals[tri[i].normal];
                }

                c.uv = uvs[tri[i].uv];
                v.emplace_back(c);
            }
            return v;
        }

        void addPoint(const vec3& p, int bone)
        {
            points.emplace_back(PointData(bone, p));
        }

        void AddUv(const vec2& u)
        {
            uvs.emplace_back(u);
        }

        std::shared_ptr<MaterialDef> addMaterial(const std::string& name)
        {
            auto mat = std::make_shared<MaterialDef>(name);
            materials.emplace(name, mat);
            return mat;
        }

        void selectMaterial(const std::string& name)
        {
            if (materials.find(name) == materials.end())
            {
                throw std::runtime_error(fmt::format("mesh does not contain a material named {}", name));
            }
            if (datas.find(name) == datas.end())
            {
                datas.emplace(name, std::make_shared<Data>());
            }
            currentd = datas[name];
        }

        void addTri(const Tri& t)
        {
            currentd->tris.emplace_back(t);
        }

        void addNomal(const vec3& v)
        {
            normals.emplace_back(v);
        }

        std::shared_ptr<CompiledMesh> Compiled()
        {
            if (compiledMesh == nullptr)
            {
                assert(false && "mesh is not compiled");
            }
            return compiledMesh;
        }

        void compile(MediaLoader* ml)
        {
#ifdef NOTYET
            compiledMesh = new CompiledMesh(ml, this);
#endif
        }

        std::shared_ptr<Bone> newBone()
        {
            auto b = std::make_shared<Bone>();
            bones.emplace_back(b);
            return b;
        }

        bool hasTrianglesFor(MaterialDef m)
        {
            auto t = std::vector<Tri>(TrianglesFor(m));
            return t.empty() == false;
        }

        void scale(float scale)
        {
            for (auto& pd : points)
            {
                pd.location = pd.location * scale;
            }

            for (auto b : bones)
            {
                b->pos = b->pos * scale;
            }
        }

        std::shared_ptr<MaterialDef> getMaterialNamed(const std::string& name)
        {
            return materials[name];
        }

        void translateFiles(const std::map<std::string, std::string>& overrides)
        {
#ifdef NOTYET
            for (auto d : Materials())
            {
                d.texture = FileSystem::MapFile(overrides, d.TextureOrName());
            }
#endif
        }
    };
}
