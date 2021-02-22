#include "engine/load/milkshape.ascii.h"

#include <vector>

#include "engine/animation.h"
#include "engine/filesystem.h"
#include "engine/load/milkshape.common.h"
#include "engine/math1.h"
#include "engine/meshdef.h"
#include "engine/strings.h"

namespace SimpleEngine::load::MilkshapeAscii
{
    // removes " at start and end
    std::string Cleanup(const std::string& p)
    {
        auto c = p.substr(1, p.length() - 2);
        return c;
    }

    struct Runner
    {
        std::shared_ptr<Model> model = std::make_shared<Model>();
        std::vector<std::string> lines;
        float scale = 1.0f;

        void run(const std::vector<std::string>& alines, float ascale)
        {
            scale = ascale;
            lines = alines;

            while (lines.size() != 0)
            {
                std::string l = readLine();

                if (StartsWith(l, "//"))
                    continue;
                auto cmd = Split(l, ':');
                if (cmd.empty())
                {
                    continue;
                }
                if (cmd[0] == "Frames")
                {
                    readFrameCount(cmd);
                }
                else if (cmd[0] == "Frame")
                {
                    readCurrentFrame(cmd);
                }
                else if (cmd[0] == "Meshes")
                {
                    readMeshes(cmd);
                }
                else if (cmd[0] == "Materials")
                {
                    readMaterials(cmd);
                }
                else if (cmd[0] == "Bones")
                {
                    readBones(cmd);
                }
            }
            model->mapBonesToId();
        }

        void readBones(const std::vector<std::string>& cmd)
        {
            int bonecount = std::stoi(cmd[1]);
            for (int boneId = 0; boneId < bonecount; ++boneId)
            {
                auto bone = model->newBone();
                bone->name = Cleanup(readLine());
                bone->parentName = Cleanup(readLine());

                const auto data = Split(readLine());
                bone->flags = std::stoi(data[0]);
                bone->x = math1::ParseFloat(data[1]) * scale;
                bone->y = math1::ParseFloat(data[2]) * scale;
                bone->z = math1::ParseFloat(data[3]) * scale;
                bone->rx = math1::ParseFloat(data[4]);
                bone->ry = math1::ParseFloat(data[5]);
                bone->rz = math1::ParseFloat(data[6]);

                readPositionFrames(bone);
                readRotationFrame(bone);
            }
        }

        void readRotationFrame(std::shared_ptr<Bone> bone)
        {
            std::string textcount = readLine();
            int count = std::stoi(textcount);
            for (int frameId = 0; frameId < count; ++frameId)
            {
                auto data = Split(readLine());
                auto key = bone->newRotationKey();
                key->time = math1::ParseFloat(data[0]);
                key->x = math1::ParseFloat(data[1]);
                key->y = math1::ParseFloat(data[2]);
                key->z = math1::ParseFloat(data[3]);
            }
        }

        void readPositionFrames(std::shared_ptr<Bone> bone)
        {
            std::string textcount = readLine();
            int count = std::stoi(textcount);
            for (int frameId = 0; frameId < count; ++frameId)
            {
                auto data = Split(readLine());
                auto key = bone->newPositionKey();
                key->time = math1::ParseFloat(data[0]);
                key->x = math1::ParseFloat(data[1]) * scale;
                key->y = math1::ParseFloat(data[2]) * scale;
                key->z = math1::ParseFloat(data[3]) * scale;
            }
        }

        void readMaterials(const std::vector<std::string>& cmd)
        {
            int materialcount = std::stoi(cmd[1]);
            for (int matId = 0; matId < materialcount; ++matId)
            {
                readSingleMaterial();
            }
        }

        void readSingleMaterial()
        {
            auto mat = model->newMaterial();
            mat->name = Cleanup(readLine());
            mat->ambient.parse(readLine());
            mat->diffuse.parse(readLine());
            mat->specular.parse(readLine());
            mat->emissive.parse(readLine());
            mat->shininess = math1::ParseFloat(readLine());
            mat->transperency = math1::ParseFloat(readLine());
            mat->diffuseTexture = Cleanup(readLine());
            mat->alphatexture = Cleanup(readLine());
        }

        void readMeshes(std::vector<std::string> cmd)
        {
            // number of meshes
            int meshcount = std::stoi(cmd[1]);
            for (int meshId = 0; meshId < meshcount; ++meshId)
            {
                readSingleMesh();
            }
        }

        void readSingleMesh()
        {
            std::string meshline = readLine();
            const auto meshdata = Split(meshline);
            auto mesh = model->newMesh();
            mesh->name = Cleanup(meshdata[0]);
            mesh->flags = std::stoi(meshdata[1]);
            mesh->materialId = std::stoi(meshdata[2]);

            std::string vertexline = readLine();
            int vertexcount = std::stoi(vertexline);
            for (int vertex = 0; vertex < vertexcount; ++vertex)
            {
                readSingleVertex(mesh);
            }

            std::string normalline = readLine();
            int normalcount = std::stoi(vertexline);
            for (int normal = 0; normal < normalcount; ++normal)
            {
                readSingleNormal(mesh);
            }

            std::string triline = readLine();
            int tricount = std::stoi(triline);
            for (int tri = 0; tri < tricount; ++tri)
            {
                readSingleTriangle(mesh);
            }
        }

        void readSingleTriangle(std::shared_ptr<Mesh> mesh)
        {
            std::string triline = readLine();
            auto tricmd = Split(triline);
            auto tri = mesh->newTri();
            tri->flags = std::stoi(tricmd[0]);
            tri->v1 = std::stoi(tricmd[1]);
            tri->v2 = std::stoi(tricmd[2]);
            tri->v3 = std::stoi(tricmd[3]);
            tri->n1 = std::stoi(tricmd[4]);
            tri->n2 = std::stoi(tricmd[5]);
            tri->n3 = std::stoi(tricmd[6]);
            tri->smoothingGroup = std::stoi(tricmd[7]);
            tri->buildNormal(*mesh);
        }

        void readSingleNormal(std::shared_ptr<Mesh> mesh)
        {
            std::string normalline = readLine();
            auto normalcmd = Split(normalline);
            auto n = mesh->newNormal();
            n->x(math1::ParseFloat(normalcmd[0]));
            n->y(math1::ParseFloat(normalcmd[1]));
            n->z(math1::ParseFloat(normalcmd[2]));
            n->normalize();
        }

        void readSingleVertex(std::shared_ptr<Mesh> mesh)
        {
            std::string vertexline = readLine();
            auto vertexcmd = Split(vertexline);
            auto v = mesh->newVertex();
            v->flags = std::stoi(vertexcmd[0]);
            v->x(math1::ParseFloat(vertexcmd[1]) * scale);
            v->y(math1::ParseFloat(vertexcmd[2]) * scale);
            v->z(math1::ParseFloat(vertexcmd[3]) * scale);
            v->u(math1::ParseFloat(vertexcmd[4]));
            v->v(math1::ParseFloat(vertexcmd[5]));
            v->bone = std::stoi(vertexcmd[6]);
        }

        std::string readLine()
        {
            const auto line = lines[0];
            lines.erase(lines.begin());
            return line;
        }

        void readCurrentFrame(const std::vector<std::string>& cmd)
        {
            model->currentFrame = std::stoi(cmd[1]);
        }

        void readFrameCount(const std::vector<std::string>& cmd)
        {
            model->framecount = std::stoi(cmd[1]);
        }
    };

    void Load(FileSystem* fs, const std::string& path, std::shared_ptr<MeshDef>* def, std::shared_ptr<Animation>* animation, float scale)
    {
        Runner runner = Runner();
        runner.run(fs->readLines(path), scale);
        *def = MilkshapeCommon::ExtractMeshDefinition(runner.model);
        *animation = MilkshapeCommon::ExtractAnimation(runner.model);
    }
}
