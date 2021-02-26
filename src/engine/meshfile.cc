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

namespace simple
{
    std::string MaterialNameFromId(int materialid)
    {
        return fmt::format("m{}", materialid);
    }

    namespace meshdef_file
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
            const auto s = fs->Open(path);
            {
                auto br = BinaryReader(s);
                int version = br.ReadInt32();
                int materials = br.ReadInt32();
                for (int materialid = 0; materialid < materials; ++materialid)
                {
                    auto m = def->AddMaterial(MaterialNameFromId(materialid));
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
                    auto bone = def->CreateNewBone();
                    bone->name = br.ReadString();
                    bone->parent = br.ReadInt32();
                    bone->position = vec3::Read(br);
                    vec3 qv = vec3::Read(br);
                    bone->rotation = quat(br.ReadSingle(), qv);
                }
                int pointcount = br.ReadInt32();
                for (int pointid = 0; pointid < pointcount; ++pointid)
                {
                    int boneid = br.ReadInt32();
                    vec3 p = vec3(br.ReadSingle(), br.ReadSingle(), br.ReadSingle());
                    def->AddPosition(p, boneid);
                }
                int uvcount = br.ReadInt32();
                for (int uvid = 0; uvid < uvcount; ++uvid)
                {
                    vec2 v = vec2(br.ReadSingle(), br.ReadSingle());
                    def->AddTextureCoordinate(v);
                }
                int normalcount = br.ReadInt32();
                for (int normalid = 0; normalid < normalcount; ++normalid)
                {
                    vec3 p = vec3(br.ReadSingle(), br.ReadSingle(), br.ReadSingle());
                    def->AddNormal(p);
                }

                for (int materialid = 0; materialid < materials; ++materialid)
                {
                    def->SelectCurrentMaterial(MaterialNameFromId(materialid));
                    int tricount = br.ReadInt32();
                    for (int triid = 0; triid < tricount; ++triid)
                    {
                        std::array<IndexedVertex, 3> data;
                        for (int i = 0; i < 3; ++i)
                        {
                            data[i].position = br.ReadInt32();
                            data[i].texture_coordinate = br.ReadInt32();
                            data[i].normal = br.ReadInt32();
                        }
                        def->AddTriangle(Triangle(data));
                    }
                }
            }
            return def;
        }

        void Write(const std::string& path, std::shared_ptr<MeshDef> def)
        {
            auto bw = BinaryWriter(path);

            bw.WriteInt32(0);
            auto materials = def->GetMaterials();
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
                bw.WriteSingle(bone->position.x);
                bw.WriteSingle(bone->position.y);
                bw.WriteSingle(bone->position.z);
                bw.WriteSingle(bone->rotation.x);
                bw.WriteSingle(bone->rotation.y);
                bw.WriteSingle(bone->rotation.z);
                bw.WriteSingle(bone->rotation.w);
            }
            bw.WriteInt32(def->positions.size());
            for (auto v : def->positions)
            {
                bw.WriteInt32(v->bone_id);
                bw.WriteSingle(v->position.x);
                bw.WriteSingle(v->position.y);
                bw.WriteSingle(v->position.z);
            }
            bw.WriteInt32(def->texture_coordinates.size());
            for (vec2 u : def->texture_coordinates)
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
                std::vector<Triangle> tris = std::vector<Triangle>(def->GetTrianglesFor(*mat));
                bw.WriteInt32(tris.size());
                for (Triangle tri : tris)
                {
                    for (int i = 0; i < 3; ++i)
                    {
                        bw.WriteSingle(tri[i].position);
                        bw.WriteSingle(tri[i].texture_coordinate);
                        bw.WriteSingle(tri[i].normal);
                    }
                }
            }
        }
    }

    namespace wavefront_obj
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
            std::shared_ptr<MaterialDefinition> mat;
            const auto file = fs->Open(path);
            {
                for (std::string l : file_util::LinesIn(file))
                {
                    std::string line = Trim(l);
                    if (line.empty() == false && line[0] != '#')
                    {
                        auto data = Split(line);
                        if (data[0] == "newmtl")
                        {
                            mat = mesh->AddMaterial(Trim(data[1]));
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
                            mat->texture = mesh_file::Resolve(path, data[1]);
                        }
                    }
                }
            }
        }

        std::shared_ptr<MeshDef> Load(FileSystem* fs, const std::string& path)
        {
            auto mesh = std::make_shared<MeshDef>();
            const auto file = fs->Open(path);
            {
                for (const std::string& l : file_util::LinesIn(file))
                {
                    std::string line = Trim(l);
                    if (line.empty() == false && line[0] != '#')
                    {
                        auto data = Split(line);
                        if (data[0] == "v")
                        {
                            mesh->AddPosition(vec3(floatParse(data[1]), floatParse(data[2]), floatParse(data[3])), -1);
                        }
                        else if (data[0] == "vt")
                        {
                            mesh->AddTextureCoordinate(vec2(floatParse(data[1]), floatParse(data[2])));
                        }
                        else if (data[0] == "vn")
                        {
                            mesh->AddNormal(vec3(floatParse(data[1]), floatParse(data[2]), floatParse(data[3])));
                        }
                        else if (data[0] == "f")
                        {
                            std::vector<IndexedVertex> vd;
                            for (int i = 1; i < data.size(); ++i)
                            {
                                auto ind = Split(data[i], '/');
                                IndexedVertex v;
                                v.position = std::stoi(ind[0]) - 1;
                                v.texture_coordinate = std::stoi(ind[1]) - 1;
                                v.normal = ind.size() > 2 ? std::stoi(ind[2]) - 1 : -1;
                                vd.emplace_back(v);
                            }
                            if (vd.size() < 3)
                                throw std::runtime_error("Face data incomplete");
                            for (int i = 2; i < vd.size(); ++i)
                            {
                                mesh->AddTriangle(Triangle{vd[0], vd[1], vd[i]});
                            }
                        }
                        else if (data[0] == "usemtl")
                        {
                            mesh->SelectCurrentMaterial(Trim(data[1]));
                        }
                        else if (data[0] == "mtllib")
                        {
                            LoadMaterialLibrary(mesh, fs, mesh_file::Resolve(path, Trim(data[1])));
                        }
                    }
                }
            }
            return mesh;
        }
    }

    namespace mesh_file
    {
        std::shared_ptr<MeshDef> Load(FileSystem* fs, const std::string& path)
        {
            if (EndsWith(path, ".obj"))
                return wavefront_obj::Load(fs, path);
            else if (EndsWith(path, ".mdf"))
            {
                auto loaded = meshdef_file::Load(fs, path);
                loaded->UntransformDefaultPose();
                return loaded;
            }
            else
                throw UserException(path + " does not use a known fileformat");
        }

        void Save(const std::string& s, std::shared_ptr<MeshDef> def)
        {
            meshdef_file::Write(s, def);
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
