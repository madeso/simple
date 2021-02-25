#include "engine/meshfile.h"

#include <cstdlib>
#include <filesystem>
#include <memory>

#include "engine/filesystem.h"
#include "engine/fileutil.h"
#include "engine/meshdef.h"
#include "engine/strings.h"
#include "engine/userexception.h"
#include "fmt/core.h"

namespace SimpleEngine
{
    std::string MaterialNameFromId(int materialid)
    {
        return fmt::format("m{}", materialid);
    }

    namespace MeshDefFile
    {
        void WriteColor(BinaryWriter& bw, const vec3& mat)
        {
            bw.WriteSingle(mat.x);
            bw.WriteSingle(mat.y);
            bw.WriteSingle(mat.z);
        }

        std::shared_ptr<MeshDef> Load(FileSystem* fs, const std::string& path)
        {
            auto def = std::make_shared<MeshDef>();
            const auto s = fs->open(path);
            {
                auto br = BinaryReader(s);
                int version = br.ReadInt32();
                int materials = br.ReadInt32();
                for (int materialid = 0; materialid < materials; ++materialid)
                {
                    auto m = def->addMaterial(MaterialNameFromId(materialid));
                    m->texture = br.ReadString();
                    m->ambient = vec3::Read(br);
                    m->diffuse = vec3::Read(br);
                    m->specular = vec3::Read(br);
                    m->emissive = vec3::Read(br);
                    m->alpha = br.ReadSingle();
                    m->shininess = br.ReadSingle();
                }
                int bonecount = br.ReadInt32();
                for (int boneid = 0; boneid < bonecount; ++boneid)
                {
                    auto bone = def->newBone();
                    bone->name = br.ReadString();
                    bone->parent = br.ReadInt32();
                    bone->pos = vec3::Read(br);
                    vec3 qv = vec3::Read(br);
                    bone->rot = quat(br.ReadSingle(), qv);
                }
                int pointcount = br.ReadInt32();
                for (int pointid = 0; pointid < pointcount; ++pointid)
                {
                    int boneid = br.ReadInt32();
                    vec3 p = vec3(br.ReadSingle(), br.ReadSingle(), br.ReadSingle());
                    def->addPoint(p, boneid);
                }
                int uvcount = br.ReadInt32();
                for (int uvid = 0; uvid < uvcount; ++uvid)
                {
                    vec2 v = vec2(br.ReadSingle(), br.ReadSingle());
                    def->AddUv(v);
                }
                int normalcount = br.ReadInt32();
                for (int normalid = 0; normalid < normalcount; ++normalid)
                {
                    vec3 p = vec3(br.ReadSingle(), br.ReadSingle(), br.ReadSingle());
                    def->addNomal(p);
                }

                for (int materialid = 0; materialid < materials; ++materialid)
                {
                    def->selectMaterial(MaterialNameFromId(materialid));
                    int tricount = br.ReadInt32();
                    for (int triid = 0; triid < tricount; ++triid)
                    {
                        std::array<VertexData, 3> data;
                        for (int i = 0; i < 3; ++i)
                        {
                            data[i].vertex = br.ReadInt32();
                            data[i].uv = br.ReadInt32();
                            data[i].normal = br.ReadInt32();
                        }
                        def->addTri(Tri(data));
                    }
                }
            }
            return def;
        }

        void Write(const std::string& path, std::shared_ptr<MeshDef> def)
        {
            auto bw = BinaryWriter(path);

            bw.WriteInt32(0);
            auto materials = def->Materials();
            bw.WriteInt32(materials.size());
            for (auto mat : materials)
            {
                bw.WriteString(mat->texture);
                WriteColor(bw, mat->ambient);
                WriteColor(bw, mat->diffuse);
                WriteColor(bw, mat->specular);
                WriteColor(bw, mat->emissive);
                bw.WriteSingle(mat->alpha);
                bw.WriteSingle(mat->shininess);
            }
            bw.WriteInt32(def->bones.size());
            for (auto bone : def->bones)
            {
                bw.WriteString(bone->name);
                bw.WriteInt32(bone->parent);
                bw.WriteSingle(bone->pos.x);
                bw.WriteSingle(bone->pos.y);
                bw.WriteSingle(bone->pos.z);
                bw.WriteSingle(bone->rot.x);
                bw.WriteSingle(bone->rot.y);
                bw.WriteSingle(bone->rot.z);
                bw.WriteSingle(bone->rot.w);
            }
            bw.WriteInt32(def->points.size());
            for (auto v : def->points)
            {
                bw.WriteInt32(v->boneid);
                bw.WriteSingle(v->location.x);
                bw.WriteSingle(v->location.y);
                bw.WriteSingle(v->location.z);
            }
            bw.WriteInt32(def->uvs.size());
            for (vec2 u : def->uvs)
            {
                bw.WriteSingle(u.x);
                bw.WriteSingle(u.y);
            }
            bw.WriteInt32(def->normals.size());
            for (vec3 n : def->normals)
            {
                bw.WriteSingle(n.x);
                bw.WriteSingle(n.y);
                bw.WriteSingle(n.z);
            }
            for (auto mat : materials)
            {
                std::vector<Tri> tris = std::vector<Tri>(def->TrianglesFor(*mat));
                bw.WriteInt32(tris.size());
                for (Tri tri : tris)
                {
                    for (int i = 0; i < 3; ++i)
                    {
                        bw.WriteSingle(tri[i].vertex);
                        bw.WriteSingle(tri[i].uv);
                        bw.WriteSingle(tri[i].normal);
                    }
                }
            }
        }
    }

    namespace WavefrontObj
    {
        float floatParse(const std::string& s)
        {
            return std::atof(s.c_str());
        }

        vec3 ParseColor(const std::vector<std::string>& data)
        {
            float r = floatParse(data[1]);
            float g = floatParse(data[1]);
            float b = floatParse(data[1]);
            return vec3(r, g, b);
        }

        void LoadMaterialLibrary(std::shared_ptr<MeshDef> mesh, FileSystem* fs, const std::string& path)
        {
            std::shared_ptr<MaterialDef> mat;
            const auto file = fs->open(path);
            {
                for (std::string l : FileUtil::LinesIn(file))
                {
                    std::string line = Trim(l);
                    if (line.empty() == false && line[0] != '#')
                    {
                        auto data = Split(line);
                        if (data[0] == "newmtl")
                        {
                            mat = mesh->addMaterial(Trim(data[1]));
                        }
                        else if (data[0] == "Ka")
                        {
                            mat->ambient = ParseColor(data);
                        }
                        else if (data[0] == "Ke")
                        {
                            mat->emissive = ParseColor(data);
                        }
                        else if (data[0] == "Kd")
                        {
                            mat->diffuse = ParseColor(data);
                        }
                        else if (data[0] == "Ks")
                        {
                            mat->specular = ParseColor(data);
                        }
                        else if (data[0] == "d" || data[0] == "Tr")
                        {
                            mat->alpha = floatParse(data[1]);
                        }
                        else if (data[0] == "Ns")
                        {
                            mat->shininess = floatParse(data[1]);
                        }
                        else if (data[0] == "map_Ka" || data[0] == "map_Kd")
                        {
                            mat->texture = MeshFile::Resolve(path, data[1]);
                        }
                    }
                }
            }
        }

        std::shared_ptr<MeshDef> Load(FileSystem* fs, const std::string& path)
        {
            auto mesh = std::make_shared<MeshDef>();
            const auto file = fs->open(path);
            {
                for (const std::string& l : FileUtil::LinesIn(file))
                {
                    std::string line = Trim(l);
                    if (line.empty() == false && line[0] != '#')
                    {
                        auto data = Split(line);
                        if (data[0] == "v")
                        {
                            mesh->addPoint(vec3(floatParse(data[1]), floatParse(data[2]), floatParse(data[3])), -1);
                        }
                        else if (data[0] == "vt")
                        {
                            mesh->AddUv(vec2(floatParse(data[1]), floatParse(data[2])));
                        }
                        else if (data[0] == "vn")
                        {
                            mesh->addNomal(vec3(floatParse(data[1]), floatParse(data[2]), floatParse(data[3])));
                        }
                        else if (data[0] == "f")
                        {
                            std::vector<VertexData> vd;
                            for (int i = 1; i < data.size(); ++i)
                            {
                                auto ind = Split(data[i], '/');
                                VertexData v;
                                v.vertex = std::stoi(ind[0]) - 1;
                                v.uv = std::stoi(ind[1]) - 1;
                                v.normal = ind.size() > 2 ? std::stoi(ind[2]) - 1 : -1;
                                vd.emplace_back(v);
                            }
                            if (vd.size() < 3)
                                throw std::runtime_error("Face data incomplete");
                            for (int i = 2; i < vd.size(); ++i)
                            {
                                mesh->addTri(Tri{vd[0], vd[1], vd[i]});
                            }
                        }
                        else if (data[0] == "usemtl")
                        {
                            mesh->selectMaterial(Trim(data[1]));
                        }
                        else if (data[0] == "mtllib")
                        {
                            LoadMaterialLibrary(mesh, fs, MeshFile::Resolve(path, Trim(data[1])));
                        }
                    }
                }
            }
            return mesh;
        }
    }

    namespace MeshFile
    {
        std::shared_ptr<MeshDef> Load(FileSystem* fs, const std::string& path)
        {
            if (EndsWith(path, ".obj"))
                return WavefrontObj::Load(fs, path);
            else if (EndsWith(path, ".mdf"))
            {
                auto loaded = MeshDefFile::Load(fs, path);
                loaded->mapBones();
                return loaded;
            }
            else
                throw UserException(path + " does not use a known fileformat");
        }

        void Save(const std::string& s, std::shared_ptr<MeshDef> def)
        {
            MeshDefFile::Write(s, def);
        }

        // baseppath = hello:world.png
        // file = C:\foobar\user.bmp
        // should return hello:user.bmp
        std::string Resolve(const std::string& basepath, const std::string& file)
        {
            return std::filesystem::path(file).filename().string();
        }
    }
}
