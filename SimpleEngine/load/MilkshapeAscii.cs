using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimpleEngine.load
{
    public static class MilkshapeAscii
    {
        public static void Load(FileSystem fs, string path, out MeshDef def, out Animation animation, float scale)
        {
            Runner runner = new Runner();
            runner.run(fs.readLines(path), scale);
            def = MilkshapeCommon.ExtractMeshDefinition(runner.model);
            animation = MilkshapeCommon.ExtractAnimation(runner.model);
        }
        
        // removes " at start and end
        private static string Cleanup(string p)
        {
            return p.Substring(1, p.Length-2);
        }
        private class Runner
        {
            internal Model model = new Model();
            List<string> lines;
            float scale = 1.0f;

            public void run(IEnumerable<string> alines, float scale)
            {
                this.scale = scale;
                lines = new List<string>(alines);

                while (lines.Count != 0)
                {
                    string l = readLine();

                    if (l.StartsWith("//")) continue;
                    string[] cmd = l.Split(":".ToCharArray());
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

            private void readBones(string[] cmd)
            {
                int bonecount = int.Parse(cmd[1]);
                for (int boneId = 0; boneId < bonecount; ++boneId)
                {
                    Bone bone = model.newBone();
                    bone.name = Cleanup(readLine());
                    bone.parentName = Cleanup(readLine());

                    string[] data = readLine().Split(" ".ToCharArray());
                    bone.flags = int.Parse(data[0]);
                    bone.x = math1.ParseFloat(data[1]) * scale;
                    bone.y = math1.ParseFloat(data[2]) * scale;
                    bone.z = math1.ParseFloat(data[3]) * scale;
                    bone.rx = math1.ParseFloat(data[4]);
                    bone.ry = math1.ParseFloat(data[5]);
                    bone.rz = math1.ParseFloat(data[6]);

                    readPositionFrames(bone);
                    readRotationFrame(bone);
                }
            }

            private void readRotationFrame(Bone bone)
            {
                string textcount = readLine();
                int count = int.Parse(textcount);
                for (int frameId = 0; frameId < count; ++frameId)
                {
                    string[] data = readLine().Split(" ".ToCharArray());
                    RotatonKey key = bone.newRotationKey();
                    key.time = math1.ParseFloat(data[0]);
                    key.x = math1.ParseFloat(data[1]);
                    key.y = math1.ParseFloat(data[2]);
                    key.z = math1.ParseFloat(data[3]);
                }
            }

            private void readPositionFrames(Bone bone)
            {
                string textcount = readLine();
                int count = int.Parse(textcount);
                for (int frameId = 0; frameId < count; ++frameId)
                {
                    string[] data = readLine().Split(" ".ToCharArray());
                    PositionKey key = bone.newPositionKey();
                    key.time = math1.ParseFloat(data[0]);
                    key.x = math1.ParseFloat(data[1]) * scale;
                    key.y = math1.ParseFloat(data[2]) * scale;
                    key.z = math1.ParseFloat(data[3]) * scale;
                }
            }

            private void readMaterials(string[] cmd)
            {
                int materialcount = int.Parse(cmd[1]);
                for(int matId = 0; matId < materialcount; ++matId)
                {
                    readSingleMaterial();
                }
            }

            private void readSingleMaterial()
            {
                Material mat = model.newMaterial();
                mat.name = Cleanup(readLine());
                mat.ambient.parse(readLine());
                mat.diffuse.parse(readLine());
                mat.specular.parse(readLine());
                mat.emissive.parse(readLine());
                mat.shininess = math1.ParseFloat(readLine());
                mat.transperency = math1.ParseFloat(readLine());
                mat.diffuseTexture = Cleanup(readLine());
                mat.alphatexture = Cleanup(readLine());
            }

            private void readMeshes(string[] cmd)
            {
                // number of meshes
                int meshcount = int.Parse(cmd[1]);
                for (int meshId = 0; meshId < meshcount; ++meshId)
                {
                    readSingleMesh();
                }
            }

            private void readSingleMesh()
            {
                string meshline = readLine();
                string[] meshdata = meshline.Split(" ".ToCharArray());
                Mesh mesh = model.newMesh();
                mesh.name = Cleanup(meshdata[0]);
                mesh.flags = int.Parse(meshdata[1]);
                mesh.materialId = int.Parse(meshdata[2]);
                
                string vertexline = readLine();
                int vertexcount = int.Parse(vertexline);
                for (int vertex = 0; vertex < vertexcount; ++vertex)
                {
                    readSingleVertex(mesh);
                }

                string normalline = readLine();
                int normalcount = int.Parse(vertexline);
                for (int normal = 0; normal < normalcount; ++normal)
                {
                    readSingleNormal(mesh);
                }

                string triline = readLine();
                int tricount = int.Parse(triline);
                for (int tri = 0; tri < tricount; ++tri)
                {
                    readSingleTriangle(mesh);
                }
            }

            private void readSingleTriangle(Mesh mesh)
            {
                string triline = readLine();
                string[] tricmd = triline.Split(" ".ToCharArray());
                Tri tri = mesh.newTri();
                tri.flags = int.Parse(tricmd[0]);
                tri.v1 = int.Parse(tricmd[1]);
                tri.v2 = int.Parse(tricmd[2]);
                tri.v3 = int.Parse(tricmd[3]);
                tri.n1 = int.Parse(tricmd[4]);
                tri.n2 = int.Parse(tricmd[5]);
                tri.n3 = int.Parse(tricmd[6]);
                tri.smoothingGroup = int.Parse(tricmd[7]);
                tri.buildNormal(mesh);
            }

            private void readSingleNormal(Mesh mesh)
            {
                string normalline = readLine();
                string[] normalcmd = normalline.Split(" ".ToCharArray());
                Normal n = mesh.newNormal();
                n.x = math1.ParseFloat(normalcmd[0]);
                n.y = math1.ParseFloat(normalcmd[1]);
                n.z = math1.ParseFloat(normalcmd[2]);
                n.normalize();
            }

            private void readSingleVertex(Mesh mesh)
            {
                string vertexline = readLine();
                string[] vertexcmd = vertexline.Split(" ".ToCharArray());
                Vertex v = mesh.newVertex();
                v.flags = int.Parse(vertexcmd[0]);
                v.x = math1.ParseFloat(vertexcmd[1]) * scale;
                v.y = math1.ParseFloat(vertexcmd[2]) * scale;
                v.z = math1.ParseFloat(vertexcmd[3]) * scale;
                v.u = math1.ParseFloat(vertexcmd[4]);
                v.v = math1.ParseFloat(vertexcmd[5]);
                v.bone = int.Parse(vertexcmd[6]);
            }

            private string readLine()
            {
                string line = lines[0];
                lines.RemoveAt(0);
                return line;
            }

            private void readCurrentFrame(string[] cmd)
            {
                model.currentFrame = int.Parse(cmd[1]);
            }

            private void readFrameCount(string[] cmd)
            {
                model.framecount = int.Parse(cmd[1]);
            }
        }
    }
}