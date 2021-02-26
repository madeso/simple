#include <memory>
#include <string>

namespace simple
{
    struct MeshDef;
    struct FileSystem;

    std::string MaterialNameFromId(int materialid);

    namespace mesh_file
    {
        std::shared_ptr<MeshDef> Load(FileSystem* fs, const std::string& path);

        void Save(const std::string& s, std::shared_ptr<MeshDef> def);

        // baseppath = hello:world.png
        // file = C:\foobar\user.bmp
        // should return hello:user.bmp
        std::string Resolve(const std::string& basepath, const std::string& file);
    }
}
