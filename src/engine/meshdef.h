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

    struct Point
    {
        int bone_id;
        vec3 location;

        Point(int b, const vec3& l);

        std::string ToString() const;
    };

    struct IndexedVertex
    {
        int vertex;
        int normal;
        int uv;
    };

    using Triangle = std::array<IndexedVertex, 3>;
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
        std::shared_ptr<Bone> parent_bone;

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

    struct MatrixAndPoints
    {
        MatrixAndPoints();

        MatrixAndPoints(const mat44& m);

        mat44 matrix;
        std::vector<std::shared_ptr<Point>> points;
    };

    // split to a builder and a definition
    // use builder to load mesh from various files
    // use def for to actually prepare to render (and faster loading)
    // meshbuilder for building mesh
    // mesdef for a optimized mesh
    // compiledmesh on gpu for rendering
    struct MeshDef
    {
        std::vector<std::shared_ptr<Point>> points;
        std::vector<vec2> texturecoordinates;
        std::vector<vec3> normals;
        std::vector<std::shared_ptr<Bone>> bones;

        std::map<std::string, std::shared_ptr<TriangleList>> parts;
        std::shared_ptr<TriangleList> current_part;

        std::map<std::string, std::shared_ptr<MaterialDefinition>> materials;

        std::shared_ptr<CompiledMesh> compiled_mesh;

        int GetTriangleCount() const;

        MeshDef& MapBones();

        std::vector<std::shared_ptr<Bone>> GetRootBones() const;

        std::vector<Triangle> GetTrianglesFor(const MaterialDefinition& material) const;

        std::vector<std::shared_ptr<MaterialDefinition>> GetMaterials() const;

        std::vector<Vertex> GetVerticesForTriangle(const Triangle& triangle) const;

        void AddPoint(const vec3& point, int bone_id);
        void AddUv(const vec2& uv);
        std::shared_ptr<MaterialDefinition> AddMaterial(const std::string& name);

        void SelectCurrentMaterial(const std::string& name);

        void AddTriangle(const Triangle& t);

        void AddNormal(const vec3& v);

        std::shared_ptr<CompiledMesh> GetCompiledMesh();

        void Compile(MediaLoader* ml);

        std::shared_ptr<Bone> CreateNewBone();

        bool HasTrianglesFor(MaterialDefinition m);

        void ScaleMeshAndBones(float scale);

        std::shared_ptr<MaterialDefinition> GetExistingMaterialNamed(const std::string& name);

        void TranslateTexturePaths(const std::map<std::string, std::string>& overrides);
    };
}
