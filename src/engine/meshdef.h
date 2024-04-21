#pragma once

#include <array>
#include <map>
#include <memory>
#include <vector>

#include "engine/mat44.h"
#include "engine/quat.h"
#include "engine/vec2.h"
#include "engine/vec3.h"

namespace simple
{
    struct CompiledMesh;
    struct MediaLoader;

    /// @addtogroup anim
    /// @{

    /// A position with a bone assigned
    struct SharedVertex
    {
        int bone_id;
        vec3 position;

        SharedVertex(int b, const vec3& l);

        std::string ToString() const;
    };

    /// index into position, normal and texture coordinate
    struct IndexedVertex
    {
        int position;
        int normal;
        int texture_coordinate;
        // bone is implicit based on vertex
    };

    /// A triangle of indexed vertices
    using Triangle = std::array<IndexedVertex, 3>;

    /// A list of triangles with indexed vertices
    using TriangleList = std::vector<Triangle>;

    /// data for position, normal and texture coordinate
    struct Vertex
    {
        vec3 position;
        vec3 normal;
        vec2 texture_coordinate;
        int bone;
    };

    /// A named bone with a default pose transformation
    struct Bone
    {
        int parent;
        std::string name;
        vec3 position;
        quat rotation;

        int index = 0;
        std::shared_ptr<Bone> parent_bone;

        std::string ToString() const;

        bool HasParent() const;

        std::vector<std::shared_ptr<Bone>> children;
    };

    /// Unloaded data for a material
    struct MaterialDefinition
    {
        MaterialDefinition(const std::string& n);

        std::string name;
        vec3 ambient = vec3(0.2f, 0.2f, 0.2f);
        vec3 diffuse = vec3(0.8f, 0.8f, 0.8f);
        vec3 specular = vec3(1, 1, 1);
        vec3 emissive = vec3(0, 0, 0);

        float alpha = 1;
        float shininess = 0;

        std::string texture;

        std::string GetTextureOrName() const;

        std::string ToString() const;
    };

    /// a matrix + a list of vertex positions with assigned bones
    struct MatrixAndSharedVertices
    {
        MatrixAndSharedVertices();

        MatrixAndSharedVertices(const mat44& m);

        mat44 matrix;
        std::vector<std::shared_ptr<SharedVertex>> shared_vertices;
    };

    /// A unloaded mesh
    struct MeshDef
    {
        std::vector<std::shared_ptr<SharedVertex>> positions;
        std::vector<vec2> texture_coordinates;
        std::vector<vec3> normals;
        std::vector<std::shared_ptr<Bone>> bones;

        std::map<std::string, std::shared_ptr<TriangleList>> parts;
        std::shared_ptr<TriangleList> current_part;

        std::map<std::string, std::shared_ptr<MaterialDefinition>> materials;

        std::shared_ptr<CompiledMesh> compiled_mesh;

        int GetTriangleCount() const;

        MeshDef& UntransformDefaultPose();
        std::vector<std::shared_ptr<Bone>> GetRootBones() const;

        std::shared_ptr<Bone> CreateNewBone();

        // bone_id is already created bone index, or -1
        void AddPosition(const vec3& point, int bone_id);
        void AddTextureCoordinate(const vec2& uv);
        void AddNormal(const vec3& v);

        std::shared_ptr<MaterialDefinition> AddMaterial(const std::string& name);
        std::shared_ptr<MaterialDefinition> GetExistingMaterialNamed(const std::string& name);
        std::vector<std::shared_ptr<MaterialDefinition>> GetMaterials() const;

        void SelectCurrentMaterial(const std::string& name);

        // Add triangle using currently selected material
        void AddTriangle(const Triangle& triangle);
        bool HasTrianglesFor(MaterialDefinition m);

        std::vector<Triangle> GetTrianglesFor(const MaterialDefinition& material) const;
        std::vector<Vertex> GetVerticesForTriangle(const Triangle& triangle) const;

        std::shared_ptr<CompiledMesh> GetCompiledMesh();
        void Compile(MediaLoader* ml);

        void ScaleMeshAndBones(float scale);

        void TranslateTexturePaths(const std::map<std::string, std::string>& overrides);
    };

    /// @}
}
