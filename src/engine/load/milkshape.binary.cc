#include "engine/load/milkshape.binary.h"

#include <fstream>
#include <vector>

#include "engine/filesystem.h"
#include "engine/load/milkshape.common.h"
#include "fmt/core.h"

#define packed

namespace simple::load::milkshape_binary
{
    using byte = unsigned char;

    namespace packed
    {
#pragma pack(push, r1, 1)
        packed struct MS3DHeader
        {
            char ID[10];
            int version;
        };

        packed struct MS3DVertex
        {
            byte flags;
            float vertex[3];
            char boneID;
            byte refCount;

            std::string ToString() const
            {
                return fmt::format("{0} ({1} {2} {3})", (int)boneID, vertex[0], vertex[1], vertex[2]);
            }
        };

        packed struct MS3DTriangle
        {
            short flags;
            short vertexIndices[3];
            float vertexNormals[9];  //[3],[3]
            float s[3];
            float t[3];
            byte smoothingGroup;
            byte groupIndex;
        };

        packed struct MS3DMaterial
        {
            char name[32];
            float ambient[4];
            float diffuse[4];
            float specular[4];
            float emissive[4];
            float shininess;     // 0.0f - 128.0f
            float transparency;  // 0.0f - 1.0f
            char mode;           // 0, 1, 2 is unused now
            char texture[128];
            char alphamap[128];
        };

        packed struct MS3DJoint
        {
            byte flags;
            char name[32];
            char parentName[32];
            float rotation[3];
            float translation[3];
            short numRotationKeyframes;
            short numTranslationKeyframes;
        };

        packed struct MS3DKeyframe
        {
            float time;
            float parameter[3];
        };
#pragma pack(pop, r1)
    }

    struct MeshGroup
    {
        std::string name;
        int material = 0;
        std::vector<int> tri;
    };

    struct Joint
    {
        MS3DJoint joint;
        std::vector<MS3DKeyframe> rotations;
        std::vector<MS3DKeyframe> translations;
    };

    std::string CropNull(const std::string& input)
    {
        return input;
    }

    std::string AsString(char* c)
    {
        return CropNull(std::string(c));
    }

    struct Runner
    {
        std::ifstream fs;

        // result
        int framecount;
        float current;
        float animfps;
        std::vector<MS3DVertex> vertices;
        std::vector<MS3DTriangle> triangles;
        std::vector<MS3DMaterial> materials;
        std::vector<MeshGroup> groups;
        std::vector<Joint> joints;

        template <typename T>
        T Read()
        {
            T t;
            fs.read((char*)&t, sizeof(T));
            return t;
        }

        int ReadCount()
        {
            return Read<short>();
        }

        std::string ReadString(int count)
        {
            std::vector<char> b;
            b.resize(count + 1, 0);
            fs.read(b.data(), sizeof(char) * count);
            b[count] = 0;
            return b.data();
        }

        Runner(const std::string& path)
            : fs(path.c_str(), std::ios::binary)
        {
            if (fs.good() == false)
            {
                throw std::runtime_error(fmt::format("Binary file not found: '{}'", path));
            }
        }

        void run()
        {
            MS3DHeader header = Read<MS3DHeader>();
            readVertices();
            readTriangles();
            readMeshGroups();
            readMaterials();
            animfps = Read<float>();
            current = Read<float>();
            framecount = Read<int>();
            readJoints();
        }

        void readJoints()
        {
            int jointcount = ReadCount();

            for (int ji = 0; ji < jointcount; ++ji)
            {
                Joint joint = Joint();
                joint.joint = Read<MS3DJoint>();
                for (int ki = 0; ki < joint.joint.numRotationKeyframes; ++ki)
                {
                    MS3DKeyframe kf = Read<MS3DKeyframe>();
                    joint.rotations.emplace_back(kf);
                }
                for (int ki = 0; ki < joint.joint.numTranslationKeyframes; ++ki)
                {
                    MS3DKeyframe kf = Read<MS3DKeyframe>();
                    joint.translations.emplace_back(kf);
                }

                joints.emplace_back(joint);
            }
        }

        void readMaterials()
        {
            int matcount = ReadCount();
            for (int m = 0; m < matcount; ++m)
            {
                auto mat = Read<MS3DMaterial>();
                materials.emplace_back(mat);
            }
        }

        void readMeshGroups()
        {
            int meshgroups = ReadCount();
            for (int m = 0; m < meshgroups; ++m)
            {
                MeshGroup group;
                byte flags = Read<byte>();
                group.name = CropNull(ReadString(32));
                int mtricount = ReadCount();
                for (int t = 0; t < mtricount; ++t)
                {
                    auto tr = Read<short>();
                    group.tri.emplace_back(tr);
                }
                group.material = Read<char>();
                groups.emplace_back(group);
            }
        }

        void readTriangles()
        {
            int tricount = ReadCount();
            for (int t = 0; t < tricount; ++t)
            {
                auto tri = Read<MS3DTriangle>();
                triangles.emplace_back(tri);
            }
        }

        void readVertices()
        {
            int vertcount = ReadCount();
            for (int v = 0; v < vertcount; ++v)
            {
                auto vt = Read<MS3DVertex>();
                vertices.emplace_back(vt);
            }
        }
    };

    void copyToColor(Color color, float* p)
    {
        color.r = p[0];
        color.g = p[1];
        color.b = p[2];
        color.a = p[3];
    }

    std::shared_ptr<Model> ExtractModel(Runner& run)
    {
        auto model = std::make_shared<Model>();
        model->frame_count = run.framecount;
        model->current_frame = run.current;

        for (MS3DMaterial s : run.materials)
        {
            auto mat = model->NewMaterial();
            mat->name = AsString(s.name);
            copyToColor(mat->ambient, s.ambient);
            copyToColor(mat->diffuse, s.diffuse);
            copyToColor(mat->specular, s.specular);
            copyToColor(mat->emissive, s.emissive);
            mat->shininess = s.shininess;
            mat->transperency = s.transparency;
            mat->diffuse_texture = AsString(s.texture);
            mat->alpha_texture = AsString(s.alphamap);
        }

        for (MeshGroup g : run.groups)
        {
            auto m = model->NewMesh();
            m->materialId = g.material;
            for (int tr : g.tri)
            {
                MS3DTriangle tri = run.triangles[tr];
                int vid[3];
                int nid[3];

                for (int i = 0; i < 3; ++i)
                {
                    vid[i] = m->vertices.size();
                    nid[i] = m->normals.size();

                    auto norm = m->NewNormal();
                    norm->SetX(tri.vertexNormals[i + 0]);
                    norm->SetY(tri.vertexNormals[i + 1]);
                    norm->SetZ(tri.vertexNormals[i + 2]);

                    MS3DVertex v = run.vertices[tri.vertexIndices[i]];
                    auto ver = m->NewVertex();
                    ver->bone = v.boneID;
                    ver->SetU(tri.s[i]);
                    ver->SetV(tri.t[i]);
                    ver->SetX(v.vertex[0]);
                    ver->SetY(v.vertex[1]);
                    ver->SetZ(v.vertex[2]);
                }

                auto t = m->NewTriangle();
                t->n1 = nid[0];
                t->n2 = nid[1];
                t->n3 = nid[2];
                t->v1 = vid[0];
                t->v2 = vid[1];
                t->v3 = vid[2];
            }
        }

        for (Joint j : run.joints)
        {
            auto b = model->NewBone();
            b->parent_name = AsString(j.joint.parentName);
            b->name = AsString(j.joint.name);
            b->rx = j.joint.rotation[0];
            b->ry = j.joint.rotation[1];
            b->rz = j.joint.rotation[2];
            b->x = j.joint.translation[0];
            b->y = j.joint.translation[1];
            b->z = j.joint.translation[2];

            for (MS3DKeyframe r : j.rotations)
            {
                auto k = b->NewRotationKey();
                k->time = r.time * run.animfps;
                k->x = r.parameter[0];
                k->y = r.parameter[1];
                k->z = r.parameter[2];
            }

            for (MS3DKeyframe t : j.translations)
            {
                auto k = b->NewPositionKey();
                k->time = t.time * run.animfps;
                k->x = t.parameter[0];
                k->y = t.parameter[1];
                k->z = t.parameter[2];
            }
        }

        model->MapBonesToId();
        return model;
    }

    void Load(FileSystem* fs, const std::string& meshpath, std::shared_ptr<MeshDef>* def, std::shared_ptr<Animation>* animation)
    {
        const auto s = fs->Open(meshpath);
        Runner run = Runner(s);
        run.run();

        auto model = ExtractModel(run);
        *def = milkshape_common::ExtractMeshDefinition(model);
        *animation = milkshape_common::ExtractAnimation(model);
    }

}
