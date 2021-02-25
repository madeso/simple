#include "engine/load/milkshape.common.h"

#include <cmath>
#include <stdexcept>

#include "engine/axisangle.h"
#include "engine/fileutil.h"
#include "engine/math1.h"
#include "engine/meshfile.h"
#include "engine/strings.h"
#include "fmt/core.h"

namespace SimpleEngine::load
{
    std::shared_ptr<Mesh> Model::newMesh()
    {
        auto m = std::make_shared<Mesh>();
        meshes.emplace_back(m);
        return m;
    }

    std::shared_ptr<Material> Model::newMaterial()
    {
        auto mat = std::make_shared<Material>();
        materials.emplace_back(mat);
        return mat;
    }

    std::shared_ptr<Bone> Model::newBone()
    {
        auto b = std::make_shared<Bone>();
        bones.emplace_back(b);
        return b;
    }

    int Model::boneId(const std::string& bone)
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

    void Model::mapBonesToId()
    {
        for (auto b : bones)
        {
            b->parentId = boneId(b->parentName);
        }
    }

    std::shared_ptr<Vertex> Mesh::newVertex()
    {
        auto v = std::make_shared<Vertex>();
        vertices.emplace_back(v);
        return v;
    }

    std::shared_ptr<Normal> Mesh::newNormal()
    {
        auto n = std::make_shared<Normal>();
        normals.emplace_back(n);
        return n;
    }

    std::shared_ptr<Tri> Mesh::newTri()
    {
        auto t = std::make_shared<Tri>();
        tris.emplace_back(t);
        return t;
    }

    void Vertex::x(float value)
    {
        pos.x = value;
    }

    float Vertex::x() const
    {
        return pos.x;
    }

    void Vertex::y(float value)
    {
        pos.y = value;
    }

    float Vertex::y() const
    {
        return pos.y;
    }

    void Vertex::z(float value)
    {
        pos.z = value;
    }

    float Vertex::z() const
    {
        return pos.z;
    }

    void Vertex::u(float value)
    {
        uv.x = value;
    }

    float Vertex::u() const
    {
        return uv.x;
    }

    void Vertex::v(float value)
    {
        uv.y = value;
    }

    float Vertex::v() const
    {
        return uv.y;
    }

    std::string Vertex::ToString() const
    {
        return fmt::format("{0} {1} / {2}", bone, pos.ToString(), uv.ToString());
    }

    void Normal::x(float value)
    {
        norm.x = value;
    }

    float Normal::x() const
    {
        return norm.x;
    }

    void Normal::y(float value)
    {
        norm.y = value;
    }

    float Normal::y() const
    {
        return norm.y;
    }

    void Normal::z(float value)
    {
        norm.z = value;
    }

    float Normal::z() const
    {
        return norm.z;
    }

    void Normal::normalize()
    {
        norm.normalize();
    }

    void Tri::buildNormal(const Mesh& mesh)
    {
        // not needed?
    }

    void Color::parse(const std::string& p)
    {
        auto data = Split(p);
        r = math1::ParseFloat(data[0]);
        g = math1::ParseFloat(data[1]);
        b = math1::ParseFloat(data[2]);
        a = math1::ParseFloat(data[3]);
    }

    std::string RotatonKey::ToString() const
    {
        return fmt::format("{0} {1}", time, MilkshapeCommon::makeQuat(vec3(x, y, z)).GetAxisAngle().ToString());
    }

    std::string Bone::ToString() const
    {
        return name + ": " + parentName;
    }

    std::vector<std::shared_ptr<PositionKey>>& Bone::PositionKeys()
    {
        return positions;
    }

    std::vector<std::shared_ptr<RotatonKey>>& Bone::RotationKeys()
    {
        return rotations;
    }

    std::shared_ptr<RotatonKey> Bone::newRotationKey()
    {
        auto key = std::make_shared<RotatonKey>();
        rotations.emplace_back(key);
        return key;
    }

    std::shared_ptr<PositionKey> Bone::newPositionKey()
    {
        auto key = std::make_shared<PositionKey>();
        positions.emplace_back(key);
        return key;
    }

    namespace MilkshapeCommon
    {
        std::shared_ptr<Animation> ExtractAnimation(std::shared_ptr<Model> model)
        {
            std::vector<AnimationForBone> ani;

            bool added = false;

            for (auto b : model->bones)
            {
                AnimationForBone a = AnimationForBone();

                for (const auto& pk : b->PositionKeys())
                {
                    a.AddPositon(pk->time, vec3(pk->x, pk->y, pk->z));
                    added = true;
                }
                for (const auto& rk : b->RotationKeys())
                {
                    a.AddRotation(rk->time, makeQuat(vec3(rk->x, rk->y, rk->z)));
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
                smat->texture = SmartTexture(mat->diffuseTexture);
                ++id;
            }

            for (auto b : model->bones)
            {
                auto bn = def->CreateNewBone();
                bn->pos = vec3(b->x, b->y, b->z);
                bn->rot = makeQuat(vec3(b->rx, b->ry, b->rz));
                bn->parent = b->parentId;
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
                    def->AddPoint(v->pos, v->bone);
                    def->AddUv(vec2(v->u(), 1 - v->v()));
                    ++vadded;
                }

                for (auto n : me->normals)
                {
                    def->AddNomal(n->norm);
                    ++nadded;
                }

                for (auto tr : me->tris)
                {
                    SimpleEngine::Triangle data;
                    data[0].uv = data[0].vertex = vbase + tr->v1;
                    data[1].uv = data[1].vertex = vbase + tr->v2;
                    data[2].uv = data[2].vertex = vbase + tr->v3;
                    data[0].normal = nbase + tr->n1;
                    data[1].normal = nbase + tr->n2;
                    data[2].normal = nbase + tr->n3;
                    def->AddTriangle(data);
                }
            }

            return def;
        }

        float sin(float s)
        {
            return std::sin(s);
        }

        float cos(float s)
        {
            return std::cos(s);
        }

        quat makeQuat(const vec3& angles)
        {
            float ang;
            float sr, sp, sy, cr, cp, cy;

            // FIXME: rescale the inputs to 1/2 angle
            float ascale = 0.5f;
            ang = angles(2) * ascale;
            sy = sin(ang);
            cy = cos(ang);
            ang = angles(1) * ascale;
            sp = sin(ang);
            cp = cos(ang);
            ang = angles(0) * ascale;
            sr = sin(ang);
            cr = cos(ang);

            float x = sr * cp * cy - cr * sp * sy;  // X
            float y = cr * sp * cy + sr * cp * sy;  // Y
            float z = cr * cp * sy - sr * sp * cy;  // Z
            float w = cr * cp * cy + sr * sp * sy;  // W
            quat a = quat(w, vec3(x, y, z)).GetNormalized();
            return a;
        }

        std::string SmartTexture(std::string p)
        {
            return FileUtil::GetFileName(p);
        }
    }
}
