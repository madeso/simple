#include <memory>
#include <string>
#include <vector>

#include "engine/animation.h"
#include "engine/meshdef.h"
#include "engine/quat.h"
#include "engine/vec2.h"
#include "engine/vec3.h"

namespace SimpleEngine::load
{
    struct Model;
    struct Mesh;
    struct Vertex;
    struct Normal;
    struct Tri;
    struct Color;
    struct Material;
    struct PositionKey;
    struct RotatonKey;
    struct Bone;

    struct Model
    {
        int framecount = 0;
        float currentFrame = 0;

        std::vector<std::shared_ptr<Mesh>> meshes;
        std::vector<std::shared_ptr<Material>> materials;
        std::vector<std::shared_ptr<Bone>> bones;

        std::shared_ptr<Mesh> newMesh();

        std::shared_ptr<Material> newMaterial();

        std::shared_ptr<Bone> newBone();

        int boneId(const std::string& bone);

        void mapBonesToId();
    };

    struct Mesh
    {
        std::string name = "";
        int flags = 0;
        int materialId = 0;

        std::vector<std::shared_ptr<Vertex>> vertices;
        std::vector<std::shared_ptr<Normal>> normals;
        std::vector<std::shared_ptr<Tri>> tris;

        std::shared_ptr<Vertex> newVertex();
        std::shared_ptr<Normal> newNormal();
        std::shared_ptr<Tri> newTri();
    };

    struct Vertex
    {
        int flags = 0;

        void x(float value);
        float x() const;

        void y(float value);
        float y() const;

        void z(float value);
        float z() const;

        void u(float value);
        float u() const;

        void v(float value);
        float v() const;

        std::string ToString() const;

        vec2 uv = vec2(0, 0);
        vec3 pos = vec3(0, 0, 0);
        int bone = 0;
    };

    struct Normal
    {
        void x(float value);
        float x() const;

        void y(float value);
        float y() const;

        void z(float value);
        float z() const;

        vec3 norm = vec3(0, 0, 0);

        void normalize();
    };

    struct Tri
    {
        int flags = 0;
        int v1 = 0;
        int v2 = 0;
        int v3 = 0;
        int n1 = 0;
        int n2 = 0;
        int n3 = 0;
        int smoothingGroup = 0;

        void buildNormal(const Mesh& mesh);
    };

    struct Color
    {
        float r = 1;
        float g = 1;
        float b = 1;
        float a = 1;

        void parse(const std::string& p);
    };

    struct Material
    {
        std::string name;
        Color ambient;
        Color diffuse;
        Color specular;
        Color emissive;
        float shininess = 0;
        float transperency = 1;
        std::string diffuseTexture;
        std::string alphatexture;
    };

    struct PositionKey
    {
        float time;
        float x;
        float y;
        float z;
    };

    struct RotatonKey
    {
        float time;
        float x;
        float y;
        float z;

        std::string ToString() const;
    };

    struct Bone
    {
        std::string name;
        std::string parentName;
        int parentId = -1;
        int flags;
        float x;
        float y;
        float z;
        float rx;
        float ry;
        float rz;

        std::string ToString() const;

        std::vector<std::shared_ptr<PositionKey>> positions;
        std::vector<std::shared_ptr<RotatonKey>> rotations;

        std::vector<std::shared_ptr<PositionKey>>& PositionKeys();

        std::vector<std::shared_ptr<RotatonKey>>& RotationKeys();

        std::shared_ptr<RotatonKey> newRotationKey();

        std::shared_ptr<PositionKey> newPositionKey();
    };

    namespace MilkshapeCommon
    {
        std::shared_ptr<Animation> ExtractAnimation(std::shared_ptr<Model> model);

        std::shared_ptr<MeshDef> ExtractMeshDefinition(std::shared_ptr<Model> model);

        float sin(float s);

        float cos(float s);

        quat makeQuat(const vec3& angles);

        std::string SmartTexture(std::string p);
    }
}
