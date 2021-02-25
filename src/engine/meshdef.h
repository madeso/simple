#pragma once

#include <array>
#include <map>
#include <memory>
#include <vector>

#include "engine/mat44.h"
#include "engine/quat.h"
#include "engine/vec2.h"
#include "engine/vec3.h"

namespace SimpleEngine
{
    struct CompiledMesh;
    struct MediaLoader;

    struct PointData
    {
        int bone_id;
        vec3 location;

        PointData(int bone, const vec3& loc);

        std::string ToString() const;
    };

    struct VertexData
    {
        int vertex;
        int normal;
        int uv;
    };

    using Triangle = std::array<VertexData, 3>;
    using TriangleList = std::vector<Triangle>;

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

        std::string ToString() const;

        bool HasParent() const;

        std::vector<std::shared_ptr<Bone>> children;
    };

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

    struct MatrixPointData
    {
        MatrixPointData();

        MatrixPointData(const mat44& mm);

        mat44 m;
        std::vector<std::shared_ptr<PointData>> pd;
    };

    struct MeshDef
    {
        std::vector<std::shared_ptr<PointData>> points;
        std::vector<vec2> uvs;
        std::vector<vec3> normals;
        std::vector<std::shared_ptr<Bone>> bones;

        std::map<std::string, std::shared_ptr<TriangleList>> datas;
        std::shared_ptr<TriangleList> current_data;

        std::map<std::string, std::shared_ptr<MaterialDefinition>> materials;

        std::shared_ptr<CompiledMesh> compiledMesh;

        int TriCount() const;

        MeshDef& mapBones();

        std::vector<std::shared_ptr<Bone>> GetRootBones() const;

        std::vector<Triangle> GetTrianglesFor(const MaterialDefinition& material) const;

        std::vector<std::shared_ptr<MaterialDefinition>> GetMaterials() const;

        std::vector<Vertex> GetVerticesForTriangle(const Triangle& tri) const;

        void AddPoint(const vec3& p, int bone);
        void AddUv(const vec2& u);
        std::shared_ptr<MaterialDefinition> AddMaterial(const std::string& name);

        void SelectCurrentMaterial(const std::string& name);

        void AddTriangle(const Triangle& t);

        void AddNomal(const vec3& v);

        std::shared_ptr<CompiledMesh> GetCompiledMesh();

        void Compile(MediaLoader* ml);

        std::shared_ptr<Bone> CreateNewBone();

        bool HasTrianglesFor(MaterialDefinition m);

        void ScaleMeshAndBones(float scale);

        std::shared_ptr<MaterialDefinition> GetExistingMaterialNamed(const std::string& name);

        void TranslateTexturePaths(const std::map<std::string, std::string>& overrides);
    };
}
