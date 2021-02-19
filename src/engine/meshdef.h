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
        int boneid;
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

        std::string ToString() const;

        bool hasParent() const;

        std::vector<std::shared_ptr<Bone>> childs;
    };

    struct MaterialDef
    {
        MaterialDef(const std::string& n);

        std::string name;
        vec3 ambient = vec3(0.2f, 0.2f, 0.2f);
        vec3 diffuse = vec3(0.8f, 0.8f, 0.8f);
        vec3 specular = vec3(1, 1, 1);
        vec3 emissive = vec3(0, 0, 0);

        float alpha = 1;
        float shininess = 0;

        std::string texture;

        std::string TextureOrName() const;

        std::string ToString() const;
    };

    struct Mpd
    {
        Mpd();

        Mpd(const mat44& mm);

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

        int TriCount() const;

        MeshDef& mapBones();

        std::vector<std::shared_ptr<Bone>> RootBones() const;

        std::vector<Tri> TrianglesFor(const MaterialDef& material) const;

        std::vector<std::shared_ptr<MaterialDef>> Materials();

        std::vector<Vertex> lookup(const Tri& tri) const;

        void addPoint(const vec3& p, int bone);

        void AddUv(const vec2& u);

        std::shared_ptr<MaterialDef> addMaterial(const std::string& name);

        void selectMaterial(const std::string& name);

        void addTri(const Tri& t);

        void addNomal(const vec3& v);

        std::shared_ptr<CompiledMesh> Compiled();

        void compile(MediaLoader* ml);

        std::shared_ptr<Bone> newBone();

        bool hasTrianglesFor(MaterialDef m);

        void scale(float scale);

        std::shared_ptr<MaterialDef> getMaterialNamed(const std::string& name);

        void translateFiles(const std::map<std::string, std::string>& overrides);
    };
}
