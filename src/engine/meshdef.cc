#include "engine/meshdef.h"

#include <cassert>
#include <iostream>
#include <stdexcept>


#include "engine/strings.h"
#include "engine/compiledmesh.h"
#include "engine/filesystem.h"
#include "engine/mat44.h"
#include "engine/matrixhelper.h"
#include "engine/quat.h"
#include "engine/vec2.h"
#include "engine/vec3.h"
#include "fmt/core.h"

namespace simple
{
    Point::Point(int b, const vec3& l)
        : bone_id(b)
        , location(l)
    {
    }

    std::string Point::ToString() const
    {
        return fmt::format("{0} {1}", bone_id, location.ToString());
    }

    std::string Bone::ToString() const
    {
        if (parent_bone == nullptr)
        {
            return name;
        }
        else
            return parent_bone->ToString() + "->" + name;
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
        return Nullstring(texture, name);
    }

    std::string MaterialDefinition::ToString() const
    {
        return fmt::format("{}: {}", name, Nullstring(texture, "<no_texture>"));
    }

    MatrixAndPoints::MatrixAndPoints()
        : matrix(mat44::Identity())
    {
    }

    MatrixAndPoints::MatrixAndPoints(const mat44& m)
        : matrix(m)
    {
    }

    int MeshDef::GetTriangleCount() const
    {
        int c = 0;
        for (const auto& d : parts)
        {
            c += d.second->size();
        }
        return c;
    }

    MeshDef& MeshDef::MapBones()
    {
        std::map<int, MatrixAndPoints> bone_to_mp;

        for (int i = 0; i < bones.size(); ++i)
        {
            int parent = bones[i]->parent;
            bones[i]->index = i;
            if (bones[i]->HasParent())
            {
                bones[i]->parent_bone = bones[parent];
                bones[i]->parent_bone->children.emplace_back(bones[i]);
            }

            auto& bone = bones[i];
            mat44 m = MatrixHelper(mat44::Identity()).Translate(-bone->pos).Rotate(-bone->rot).AsMat44();
            bone_to_mp.emplace(i, MatrixAndPoints{m});
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
            auto& mpd = bone_to_mp[pd->bone_id];
            mpd.points.emplace_back(pd);
        }

        for (auto& k : bone_to_mp)
        {
            const auto current_bone = bones[k.first];
            auto m = mat44::Identity();
            for (auto loop_bone = current_bone; loop_bone != nullptr; loop_bone = loop_bone->parent_bone)
            {
                m = m * bone_to_mp[loop_bone->index].matrix;
            }

            for (const auto& pd : k.second.points)
            {
                pd->location = m * pd->location;
            }
        }

        return *this;
    }

    std::vector<std::shared_ptr<Bone>> MeshDef::GetRootBones() const
    {
        std::vector<std::shared_ptr<Bone>> r;

        for (const auto& bone : bones)
        {
            if (bone->parent_bone == nullptr)
            {
                r.emplace_back(bone);
            }
        }

        return r;
    }

    std::vector<Triangle> MeshDef::GetTrianglesFor(const MaterialDefinition& material) const
    {
        const auto found_part = parts.find(material.name);

        if (found_part == parts.end())
        {
            return {};
        }
        else
        {
            return *found_part->second;
        }
    }

    std::vector<std::shared_ptr<MaterialDefinition>> MeshDef::GetMaterials() const
    {
        std::vector<std::shared_ptr<MaterialDefinition>> r;
        for (auto& material_pair : materials)
        {
            r.emplace_back(material_pair.second);
        }
        return r;
    }

    std::vector<Vertex> MeshDef::GetVerticesForTriangle(const Triangle& triangle) const
    {
        std::vector<Vertex> v;
        for (int triangle_index = 0; triangle_index < 3; ++triangle_index)
        {
            Vertex vertex;
            vertex.vertex = points[triangle[triangle_index].vertex]->location;
            vertex.bone = points[triangle[triangle_index].vertex]->bone_id;

            if (triangle[triangle_index].normal != -1)
            {
                vertex.normal = normals[triangle[triangle_index].normal];
            }

            vertex.uv = texturecoordinates[triangle[triangle_index].uv];
            v.emplace_back(vertex);
        }
        return v;
    }

    void MeshDef::AddPoint(const vec3& point, int bone_id)
    {
        points.emplace_back(std::make_shared<Point>(bone_id, point));
    }

    void MeshDef::AddUv(const vec2& uv)
    {
        texturecoordinates.emplace_back(uv);
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
        if (parts.find(name) == parts.end())
        {
            parts.emplace(name, std::make_shared<TriangleList>());
        }
        current_part = parts[name];
    }

    void MeshDef::AddTriangle(const Triangle& t)
    {
        assert(current_part);
        current_part->emplace_back(t);
    }

    void MeshDef::AddNormal(const vec3& v)
    {
        normals.emplace_back(v);
    }

    std::shared_ptr<CompiledMesh> MeshDef::GetCompiledMesh()
    {
        if (compiled_mesh == nullptr)
        {
            assert(false && "mesh is not compiled");
        }
        return compiled_mesh;
    }

    void MeshDef::Compile(MediaLoader* ml)
    {
        compiled_mesh = std::make_shared<CompiledMesh>(ml, this);
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
