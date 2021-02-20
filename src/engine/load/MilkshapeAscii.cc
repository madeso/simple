using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimpleEngine.load
{
    namespace MilkshapeAscii
    {
        static void Load(FileSystem fs, std::string path, out MeshDef def, out Animation animation, float scale)
        {
            Runner runner = Runner();
            runner.run(fs.readLines(path), scale);
            def = MilkshapeCommon.ExtractMeshDefinition(runner.model);
            animation = MilkshapeCommon.ExtractAnimation(runner.model);
        }

        // removes " at start and end

        static std::string Cleanup(std::string p)
        {
            return p.Substring(1, p.Length - 2);
        }

        struct Runner
        {
            Model model = Model();
            std::vector<std::string> lines;
            float scale = 1.0f;

            void run(IEnumerable<std::string> alines, float scale)
            {
                this.scale = scale;
                lines = std::vector<std::string>(alines);

                while (lines.Count != 0)
                {
                    std::string l = readLine();

                    if (l.StartsWith("//"))
                        continue;
                    std::string[] cmd = l.Split(":".ToCharArray());
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
                model.mapBonesToId();
            }

            void readBones(std::string[] cmd)
            {
                int bonecount = std::stoi(cmd[1]);
                for (int boneId = 0; boneId < bonecount; ++boneId)
                {
                    Bone bone = model.newBone();
                    bone.name = Cleanup(readLine());
                    bone.parentName = Cleanup(readLine());

                    std::string[] data = readLine().Split(" ".ToCharArray());
                    bone.flags = std::stoi(data[0]);
                    bone.x = math1::ParseFloat(data[1]) * scale;
                    bone.y = math1::ParseFloat(data[2]) * scale;
                    bone.z = math1::ParseFloat(data[3]) * scale;
                    bone.rx = math1::ParseFloat(data[4]);
                    bone.ry = math1::ParseFloat(data[5]);
                    bone.rz = math1::ParseFloat(data[6]);

                    readPositionFrames(bone);
                    readRotationFrame(bone);
                }
            }

            void readRotationFrame(Bone bone)
            {
                std::string textcount = readLine();
                int count = std::stoi(textcount);
                for (int frameId = 0; frameId < count; ++frameId)
                {
                    std::string[] data = readLine().Split(" ".ToCharArray());
                    RotatonKey key = bone.newRotationKey();
                    key.time = math1::ParseFloat(data[0]);
                    key.x = math1::ParseFloat(data[1]);
                    key.y = math1::ParseFloat(data[2]);
                    key.z = math1::ParseFloat(data[3]);
                }
            }

            void readPositionFrames(Bone bone)
            {
                std::string textcount = readLine();
                int count = std::stoi(textcount);
                for (int frameId = 0; frameId < count; ++frameId)
                {
                    std::string[] data = readLine().Split(" ".ToCharArray());
                    PositionKey key = bone.newPositionKey();
                    key.time = math1::ParseFloat(data[0]);
                    key.x = math1::ParseFloat(data[1]) * scale;
                    key.y = math1::ParseFloat(data[2]) * scale;
                    key.z = math1::ParseFloat(data[3]) * scale;
                }
            }

            void readMaterials(std::string[] cmd)
            {
                int materialcount = std::stoi(cmd[1]);
                for (int matId = 0; matId < materialcount; ++matId)
                {
                    readSingleMaterial();
                }
            }

            void readSingleMaterial()
            {
                Material mat = model.newMaterial();
                mat.name = Cleanup(readLine());
                mat.ambient.parse(readLine());
                mat.diffuse.parse(readLine());
                mat.specular.parse(readLine());
                mat.emissive.parse(readLine());
                mat.shininess = math1::ParseFloat(readLine());
                mat.transperency = math1::ParseFloat(readLine());
                mat.diffuseTexture = Cleanup(readLine());
                mat.alphatexture = Cleanup(readLine());
            }

            void readMeshes(std::string[] cmd)
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
                std::string[] meshdata = meshline.Split(" ".ToCharArray());
                Mesh mesh = model.newMesh();
                mesh.name = Cleanup(meshdata[0]);
                mesh.flags = std::stoi(meshdata[1]);
                mesh.materialId = std::stoi(meshdata[2]);

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

            void readSingleTriangle(Mesh mesh)
            {
                std::string triline = readLine();
                std::string[] tricmd = triline.Split(" ".ToCharArray());
                Tri tri = mesh.newTri();
                tri.flags = std::stoi(tricmd[0]);
                tri.v1 = std::stoi(tricmd[1]);
                tri.v2 = std::stoi(tricmd[2]);
                tri.v3 = std::stoi(tricmd[3]);
                tri.n1 = std::stoi(tricmd[4]);
                tri.n2 = std::stoi(tricmd[5]);
                tri.n3 = std::stoi(tricmd[6]);
                tri.smoothingGroup = std::stoi(tricmd[7]);
                tri.buildNormal(mesh);
            }

            void readSingleNormal(Mesh mesh)
            {
                std::string normalline = readLine();
                std::string[] normalcmd = normalline.Split(" ".ToCharArray());
                Normal n = mesh.newNormal();
                n.x = math1::ParseFloat(normalcmd[0]);
                n.y = math1::ParseFloat(normalcmd[1]);
                n.z = math1::ParseFloat(normalcmd[2]);
                n.normalize();
            }

            void readSingleVertex(Mesh mesh)
            {
                std::string vertexline = readLine();
                std::string[] vertexcmd = vertexline.Split(" ".ToCharArray());
                Vertex v = mesh.newVertex();
                v.flags = std::stoi(vertexcmd[0]);
                v.x = math1::ParseFloat(vertexcmd[1]) * scale;
                v.y = math1::ParseFloat(vertexcmd[2]) * scale;
                v.z = math1::ParseFloat(vertexcmd[3]) * scale;
                v.u = math1::ParseFloat(vertexcmd[4]);
                v.v = math1::ParseFloat(vertexcmd[5]);
                v.bone = std::stoi(vertexcmd[6]);
            }

            std::string readLine()
            {
                std::string line = lines[0];
                lines.RemoveAt(0);
                return line;
            }

            void readCurrentFrame(std::string[] cmd)
            {
                model.currentFrame = std::stoi(cmd[1]);
            }

            void readFrameCount(std::string[] cmd)
            {
                model.framecount = std::stoi(cmd[1]);
            }
        }
    }
}
