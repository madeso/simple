#include <memory>
#include <string>
#include <vector>

#include "engine/animation.h"
#include "engine/meshdef.h"
#include "engine/quat.h"
#include "engine/vec2.h"
#include "engine/vec3.h"

namespace simple::load
{
    /// @addtogroup load
    /// @{

    struct Model;
    struct Mesh;
    struct Vertex;
    struct Normal;
    struct Triangle;
    struct Color;
    struct Material;
    struct PositionKey;
    struct RotationKey;
    struct Bone;

    struct Model
    {
        int frame_count = 0;
        float current_frame = 0;

        std::vector<std::shared_ptr<Mesh>> meshes;
        std::vector<std::shared_ptr<Material>> materials;
        std::vector<std::shared_ptr<Bone>> bones;

        std::shared_ptr<Mesh> NewMesh();
        std::shared_ptr<Material> NewMaterial();
        std::shared_ptr<Bone> NewBone();
        int GetIdForBone(const std::string& bone);
        void MapBonesToId();
    };

    struct Mesh
    {
        std::string name;
        int flags = 0;
        int materialId = 0;

        std::vector<std::shared_ptr<Vertex>> vertices;
        std::vector<std::shared_ptr<Normal>> normals;
        std::vector<std::shared_ptr<Triangle>> triangles;

        std::shared_ptr<Vertex> NewVertex();
        std::shared_ptr<Normal> NewNormal();
        std::shared_ptr<Triangle> NewTriangle();
    };

    struct Vertex
    {
        int flags = 0;
        vec2 uv = vec2(0, 0);
        vec3 position = vec3(0, 0, 0);
        int bone = 0;

        void SetX(float value);
        float GetX() const;

        void SetY(float value);
        float GetY() const;

        void SetZ(float value);
        float GetZ() const;

        void SetU(float value);
        float GetU() const;

        void SetV(float value);
        float GetV() const;

        std::string ToString() const;
    };

    struct Normal
    {
        vec3 normal = vec3(0, 0, 0);

        void SetX(float value);
        float GetX() const;

        void SetY(float value);
        float GetY() const;

        void SetZ(float value);
        float GetZ() const;

        void Normalize();
    };

    struct Triangle
    {
        int flags = 0;
        int v1 = 0;
        int v2 = 0;
        int v3 = 0;
        int n1 = 0;
        int n2 = 0;
        int n3 = 0;
        int smoothingGroup = 0;

        void BuildNormal(const Mesh& mesh);
    };

    struct Color
    {
        float r = 1;
        float g = 1;
        float b = 1;
        float a = 1;

        void Parse(const std::string& p);
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
        std::string diffuse_texture;
        std::string alpha_texture;
    };

    struct PositionKey
    {
        float time = 0.0f;
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;
    };

    struct RotationKey
    {
        float time = 0.0f;
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;

        std::string ToString() const;
    };

    struct Bone
    {
        std::string name;
        std::string parent_name;
        int parent_id = -1;
        int flags = 0;
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;
        float rx = 0.0f;
        float ry = 0.0f;
        float rz = 0.0f;

        std::string ToString() const;

        std::vector<std::shared_ptr<PositionKey>> positions;
        std::vector<std::shared_ptr<RotationKey>> rotations;
        std::shared_ptr<RotationKey> NewRotationKey();
        std::shared_ptr<PositionKey> NewPositionKey();
    };

    namespace milkshape_common
    {
        std::shared_ptr<Animation> ExtractAnimation(std::shared_ptr<Model> model);
        std::shared_ptr<MeshDef> ExtractMeshDefinition(std::shared_ptr<Model> model);
        float Sin(float s);
        float Cos(float s);
        quat MakeQuat(const vec3& angles);
        std::string SmartTexture(std::string p);
    }

    /// @}
}
