#include "engine/meshdef.h"

#include <cassert>
#include <iostream>
#include <stdexcept>

#include "engine/compiledmesh.h"
#include "engine/filesystem.h"
#include "engine/mat44.h"
#include "engine/matrixhelper.h"
#include "engine/quat.h"
#include "engine/vec2.h"
#include "engine/vec3.h"
#include "fmt/core.h"

namespace SimpleEngine
{
    PointData::PointData(int bone, const vec3& loc)
        : bone_id(bone)
        , location(loc)
    {
    }

    std::string PointData::ToString() const
    {
        return fmt::format("{0} {1}", bone_id, location.ToString());
    }

    std::string Bone::ToString() const
    {
        if (parentBone == nullptr)
        {
            return name;
        }
        else
            return parentBone->ToString() + "->" + name;
    }

    bool Bone::HasParent() const
    {
        return parent != -1;
    }

    MaterialDefinition::MaterialDefinition(const std::string& n)
        : name(n)
    {
    }

    std::string MaterialDefinition::GetTextureOrName() const
    {
        if (texture == "")
            return name;
        else
            return texture;
    }

    std::string MaterialDefinition::ToString() const
    {
        return name + ": " + texture;
    }

    MatrixPointData::MatrixPointData()
        : m(mat44::Identity())
    {
    }

    MatrixPointData::MatrixPointData(const mat44& mm)
        : m(mm)
    {
    }

    int MeshDef::TriCount() const
    {
        int c = 0;
        for (const auto& d : datas)
        {
            c += d.second->size();
        }
        return c;
    }

    MeshDef& MeshDef::mapBones()
    {
        std::map<int, MatrixPointData> bdp;

        for (int i = 0; i < bones.size(); ++i)
        {
            int parent = bones[i]->parent;
            bones[i]->index = i;
            if (bones[i]->HasParent())
            {
                bones[i]->parentBone = bones[parent];
                bones[i]->parentBone->children.emplace_back(bones[i]);
            }

            auto& bone = bones[i];
            mat44 m = MatrixHelper(mat44::Identity()).Translate(-bone->pos).Rotate(-bone->rot).mat44();
            bdp.emplace(i, MatrixPointData{m});
        }

        for (auto& pd : points)
        {
            if (pd->bone_id == -1)
                continue;
            if (pd->bone_id >= bones.size())
            {
                throw std::runtime_error(fmt::format("bone id {} greater than bones {}", pd->bone_id, bones.size()));
            }
            const auto bone = bones[pd->bone_id];
            MatrixPointData& mpd = bdp[pd->bone_id];
            mpd.pd.emplace_back(pd);
        }

        for (auto& k : bdp)
        {
            auto bone = bones[k.first];
            mat44 m = mat44::Identity();
            for (auto b = bone; b != nullptr; b = b->parentBone)
            {
                m = m * bdp[b->index].m;
            }

            for (auto pd : k.second.pd)
            {
                pd->location = m * pd->location;
            }
        }

        return *this;
    }

    std::vector<std::shared_ptr<Bone>> MeshDef::GetRootBones() const
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

    std::vector<Triangle> MeshDef::GetTrianglesFor(const MaterialDefinition& material) const
    {
        std::vector<Triangle> r;

        if (auto found = datas.find(material.name); found != datas.end())
        {
            for (const auto& t : *found->second)
            {
                r.emplace_back(t);
            }
        }

        return r;
    }

    std::vector<std::shared_ptr<MaterialDefinition>> MeshDef::GetMaterials() const
    {
        std::vector<std::shared_ptr<MaterialDefinition>> r;
        for (auto& k : materials)
        {
            r.emplace_back(k.second);
        }
        return r;
    }

    std::vector<Vertex> MeshDef::GetVerticesForTriangle(const Triangle& tri) const
    {
        std::vector<Vertex> v;
        for (int i = 0; i < 3; ++i)
        {
            Vertex c;
            c.vertex = points[tri[i].vertex]->location;
            c.bone = points[tri[i].vertex]->bone_id;

            if (tri[i].normal != -1)
            {
                c.normal = normals[tri[i].normal];
            }

            c.uv = uvs[tri[i].uv];
            v.emplace_back(c);
        }
        return v;
    }

    void MeshDef::AddPoint(const vec3& p, int bone)
    {
        points.emplace_back(std::make_shared<PointData>(bone, p));
    }

    void MeshDef::AddUv(const vec2& u)
    {
        uvs.emplace_back(u);
    }

    std::shared_ptr<MaterialDefinition> MeshDef::AddMaterial(const std::string& name)
    {
        auto mat = std::make_shared<MaterialDefinition>(name);
        materials.emplace(name, mat);
        return mat;
    }

    void MeshDef::SelectCurrentMaterial(const std::string& name)
    {
        if (materials.find(name) == materials.end())
        {
            throw std::runtime_error(fmt::format("mesh does not contain a material named {}", name));
        }
        if (datas.find(name) == datas.end())
        {
            datas.emplace(name, std::make_shared<TriangleList>());
        }
        current_data = datas[name];
    }

    void MeshDef::AddTriangle(const Triangle& t)
    {
        assert(current_data);
        current_data->emplace_back(t);
    }

    void MeshDef::AddNomal(const vec3& v)
    {
        normals.emplace_back(v);
    }

    std::shared_ptr<CompiledMesh> MeshDef::GetCompiledMesh()
    {
        if (compiledMesh == nullptr)
        {
            assert(false && "mesh is not compiled");
        }
        return compiledMesh;
    }

    void MeshDef::Compile(MediaLoader* ml)
    {
        compiledMesh = std::make_shared<CompiledMesh>(ml, this);
    }

    std::shared_ptr<Bone> MeshDef::CreateNewBone()
    {
        auto b = std::make_shared<Bone>();
        bones.emplace_back(b);
        return b;
    }

    bool MeshDef::HasTrianglesFor(MaterialDefinition m)
    {
        auto t = std::vector<Triangle>(GetTrianglesFor(m));
        return t.empty() == false;
    }

    void MeshDef::ScaleMeshAndBones(float scale)
    {
        for (auto& pd : points)
        {
            pd->location = pd->location * scale;
        }

        for (auto b : bones)
        {
            b->pos = b->pos * scale;
        }
    }

    std::shared_ptr<MaterialDefinition> MeshDef::GetExistingMaterialNamed(const std::string& name)
    {
        return materials[name];
    }

    void MeshDef::TranslateTexturePaths(const std::map<std::string, std::string>& overrides)
    {
        for (auto d : GetMaterials())
        {
            d->texture = FileSystem::MapFile(overrides, d->GetTextureOrName());
        }
    }
}
