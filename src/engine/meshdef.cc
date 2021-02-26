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
    SharedVertex::SharedVertex(int b, const vec3& l)
        : bone_id(b)
        , position(l)
    {
    }

    std::string SharedVertex::ToString() const
    {
        return fmt::format("{0} {1}", bone_id, position.ToString());
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

    MatrixAndSharedVertices::MatrixAndSharedVertices()
        : matrix(mat44::Identity())
    {
    }

    MatrixAndSharedVertices::MatrixAndSharedVertices(const mat44& m)
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

    MeshDef& MeshDef::UntransformDefaultPose()
    {
        std::map<int, MatrixAndSharedVertices> bone_to_mp;

        // collect all bones and the reverse transformation matrix for the default pose
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
            mat44 m = MatrixHelper(mat44::Identity()).Translate(-bone->position).Rotate(-bone->rotation).AsMat44();
            bone_to_mp.emplace(i, MatrixAndSharedVertices{m});
        }

        // collect all vertices for a each bone... is this really needed? we could easily do this at the end instead
        for (auto& vertex : positions)
        {
            if (vertex->bone_id == -1)
                continue;
            if (vertex->bone_id >= bones.size())
            {
                throw std::runtime_error(fmt::format("bone id {} greater than bones {}", vertex->bone_id, bones.size()));
            }
            auto& mpd = bone_to_mp[vertex->bone_id];
            mpd.shared_vertices.emplace_back(vertex);
        }

        for (auto& k : bone_to_mp)
        {
            const auto current_bone = bones[k.first];

            // for the current bone, combine the local reverse matrices into a total reverse transformation matrix
            auto m = mat44::Identity();
            for (auto loop_bone = current_bone; loop_bone != nullptr; loop_bone = loop_bone->parent_bone)
            {
                m = m * bone_to_mp[loop_bone->index].matrix;
            }

            // finally transform each vertex from the default pose to a "no-pose pose"
            for (const auto& pd : k.second.shared_vertices)
            {
                pd->position = m * pd->position;
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
            vertex.position = positions[triangle[triangle_index].position]->position;
            vertex.bone = positions[triangle[triangle_index].position]->bone_id;

            if (triangle[triangle_index].normal != -1)
            {
                vertex.normal = normals[triangle[triangle_index].normal];
            }

            vertex.texture_coordinate = texture_coordinates[triangle[triangle_index].texture_coordinate];
            v.emplace_back(vertex);
        }
        return v;
    }

    void MeshDef::AddPosition(const vec3& point, int bone_id)
    {
        positions.emplace_back(std::make_shared<SharedVertex>(bone_id, point));
    }

    void MeshDef::AddTextureCoordinate(const vec2& uv)
    {
        texture_coordinates.emplace_back(uv);
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

    void MeshDef::AddTriangle(const Triangle& triangle)
    {
        assert(current_part);
        current_part->emplace_back(triangle);
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
        for (auto& pd : positions)
        {
            pd->position = pd->position * scale;
        }

        for (auto b : bones)
        {
            b->position = b->position * scale;
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
