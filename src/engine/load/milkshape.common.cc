#include "engine/load/milkshape.common.h"

#include <cmath>
#include <stdexcept>

#include "engine/axisangle.h"
#include "engine/fileutil.h"
#include "engine/math.h"
#include "engine/meshfile.h"
#include "engine/strings.h"
#include "fmt/core.h"

namespace simple::load
{
    std::shared_ptr<Mesh> Model::NewMesh()
    {
        auto m = std::make_shared<Mesh>();
        meshes.emplace_back(m);
        return m;
    }

    std::shared_ptr<Material> Model::NewMaterial()
    {
        auto mat = std::make_shared<Material>();
        materials.emplace_back(mat);
        return mat;
    }

    std::shared_ptr<Bone> Model::NewBone()
    {
        auto b = std::make_shared<Bone>();
        bones.emplace_back(b);
        return b;
    }

    int Model::GetIdForBone(const std::string& bone)
    {
        if (bone == "")
            return -1;
        for (int i = 0; i < bones.size(); ++i)
        {
            if (bones[i]->name == bone)
                return i;
        }
        throw std::runtime_error(fmt::format("Failed to find a match for bone '{}'", bone));
    }

    void Model::MapBonesToId()
    {
        for (auto b : bones)
        {
            b->parent_id = GetIdForBone(b->parent_name);
        }
    }

    std::shared_ptr<Vertex> Mesh::NewVertex()
    {
        auto v = std::make_shared<Vertex>();
        vertices.emplace_back(v);
        return v;
    }

    std::shared_ptr<Normal> Mesh::NewNormal()
    {
        auto n = std::make_shared<Normal>();
        normals.emplace_back(n);
        return n;
    }

    std::shared_ptr<Triangle> Mesh::NewTriangle()
    {
        auto t = std::make_shared<Triangle>();
        triangles.emplace_back(t);
        return t;
    }

    void Vertex::SetX(float value)
    {
        position.x = value;
    }

    float Vertex::GetX() const
    {
        return position.x;
    }

    void Vertex::SetY(float value)
    {
        position.y = value;
    }

    float Vertex::GetY() const
    {
        return position.y;
    }

    void Vertex::SetZ(float value)
    {
        position.z = value;
    }

    float Vertex::GetZ() const
    {
        return position.z;
    }

    void Vertex::SetU(float value)
    {
        uv.x = value;
    }

    float Vertex::GetU() const
    {
        return uv.x;
    }

    void Vertex::SetV(float value)
    {
        uv.y = value;
    }

    float Vertex::GetV() const
    {
        return uv.y;
    }

    std::string Vertex::ToString() const
    {
        return fmt::format("{0} {1} / {2}", bone, position.ToString(), uv.ToString());
    }

    void Normal::SetX(float value)
    {
        normal.x = value;
    }

    float Normal::GetX() const
    {
        return normal.x;
    }

    void Normal::SetY(float value)
    {
        normal.y = value;
    }

    float Normal::GetY() const
    {
        return normal.y;
    }

    void Normal::SetZ(float value)
    {
        normal.z = value;
    }

    float Normal::GetZ() const
    {
        return normal.z;
    }

    void Normal::Normalize()
    {
        normal.Normalize();
    }

    void Triangle::BuildNormal(const Mesh& mesh)
    {
        // not needed?
    }

    void Color::Parse(const std::string& p)
    {
        auto data = Split(p);
        r = math::ParseFloat(data[0]);
        g = math::ParseFloat(data[1]);
        b = math::ParseFloat(data[2]);
        a = math::ParseFloat(data[3]);
    }

    std::string RotationKey::ToString() const
    {
        return fmt::format("{0} {1}", time, milkshape_common::MakeQuat(vec3(x, y, z)).GetAxisAngle().ToString());
    }

    std::string Bone::ToString() const
    {
        return name + ": " + parent_name;
    }

    std::shared_ptr<RotationKey> Bone::NewRotationKey()
    {
        auto key = std::make_shared<RotationKey>();
        rotations.emplace_back(key);
        return key;
    }

    std::shared_ptr<PositionKey> Bone::NewPositionKey()
    {
        auto key = std::make_shared<PositionKey>();
        positions.emplace_back(key);
        return key;
    }

    namespace milkshape_common
    {
        std::shared_ptr<Animation> ExtractAnimation(std::shared_ptr<Model> model)
        {
            std::vector<AnimationForBone> ani;

            bool added = false;

            for (auto b : model->bones)
            {
                AnimationForBone a = AnimationForBone();

                for (const auto& pk : b->positions)
                {
                    a.AddPositon(pk->time, vec3(pk->x, pk->y, pk->z));
                    added = true;
                }
                for (const auto& rk : b->rotations)
                {
                    a.AddRotation(rk->time, MakeQuat(vec3(rk->x, rk->y, rk->z)));
                    added = true;
                }

                ani.emplace_back(a);
            }

            if (added)
                return std::make_shared<Animation>(ani);
            else
                return nullptr;
        }

        std::shared_ptr<MeshDef> ExtractMeshDefinition(std::shared_ptr<Model> model)
        {
            auto def = std::make_shared<MeshDef>();
            int id = 0;
            for (auto mat : model->materials)
            {
                auto smat = def->AddMaterial(MaterialNameFromId(id));
                smat->texture = SmartTexture(mat->diffuse_texture);
                ++id;
            }

            for (auto b : model->bones)
            {
                auto bn = def->CreateNewBone();
                bn->position = vec3(b->x, b->y, b->z);
                bn->rotation = MakeQuat(vec3(b->rx, b->ry, b->rz));
                bn->parent = b->parent_id;
                bn->name = b->name;
            }

            int vadded = 0;
            int vbase = 0;
            int nadded = 0;
            int nbase = 0;
            for (auto me : model->meshes)
            {
                vbase += vadded;
                vadded = 0;
                nbase += nadded;
                nadded = 0;

                def->SelectCurrentMaterial(MaterialNameFromId(me->materialId));
                for (auto v : me->vertices)
                {
                    def->AddPosition(v->position, v->bone);
                    def->AddTextureCoordinate(vec2(v->GetU(), 1 - v->GetV()));
                    ++vadded;
                }

                for (auto n : me->normals)
                {
                    def->AddNormal(n->normal);
                    ++nadded;
                }

                for (auto tr : me->triangles)
                {
                    simple::Triangle data;
                    data[0].texture_coordinate = data[0].position = vbase + tr->v1;
                    data[1].texture_coordinate = data[1].position = vbase + tr->v2;
                    data[2].texture_coordinate = data[2].position = vbase + tr->v3;
                    data[0].normal = nbase + tr->n1;
                    data[1].normal = nbase + tr->n2;
                    data[2].normal = nbase + tr->n3;
                    def->AddTriangle(data);
                }
            }

            return def;
        }

        float Sin(float s)
        {
            return std::sin(s);
        }

        float Cos(float s)
        {
            return std::cos(s);
        }

        quat MakeQuat(const vec3& angles)
        {
            float ang;
            float sr, sp, sy, cr, cp, cy;

            // FIXME: rescale the inputs to 1/2 angle
            float ascale = 0.5f;
            ang = angles(2) * ascale;
            sy = Sin(ang);
            cy = Cos(ang);
            ang = angles(1) * ascale;
            sp = Sin(ang);
            cp = Cos(ang);
            ang = angles(0) * ascale;
            sr = Sin(ang);
            cr = Cos(ang);

            float x = sr * cp * cy - cr * sp * sy;  // X
            float y = cr * sp * cy + sr * cp * sy;  // Y
            float z = cr * cp * sy - sr * sp * cy;  // Z
            float w = cr * cp * cy + sr * sp * sy;  // W
            quat a = quat(w, vec3(x, y, z)).GetNormalized();
            return a;
        }

        std::string SmartTexture(std::string p)
        {
            return file_util::GetFileName(p);
        }
    }
}
