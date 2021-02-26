#include "engine/filesystem.h"

#include <cassert>
#include <filesystem>
#include <fstream>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

#include "engine/fileutil.h"
#include "engine/strings.h"
#include "fmt/core.h"

namespace simple
{
    namespace fs = std::filesystem;

    FileSystem::FileSystem()
    {
    }

    void FileSystem::AddRoot(const std::string& root)
    {
        roots.emplace_back(root);
    }

    void FileSystem::AddDefaultRoots(const std::string& app, const std::string& game)
    {
        AddRoot(fs::current_path().string());
        // addRoot(combine(Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData), app, game));
        // addRoot(combine(Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData), app, game));
        // addRoot(combine(Environment.GetFolderPath(Environment.SpecialFolder.CommonApplicationData), app, game));
        // addRoot(combine(Environment.GetFolderPath(Environment.SpecialFolder.ProgramFiles), app, game));
        // addRoot(combine(Environment.GetFolderPath(Environment.SpecialFolder.Programs), app, game));
    }

    std::string FileSystem::OpenExactPath(const std::string& path) const
    {
        if (path.find("..") != std::string::npos)
        {
            throw std::runtime_error("do not specify parent directory");
        }
        for (auto& r : roots)
        {
            auto fp = fs::path{r} / path;
            if (fs::exists(fp))
            {
                return fp.string();
            }
        }
        throw std::runtime_error(fmt::format("File not found in VFS: '{}'", path));
    }

    std::string FileSystem::Open(const std::string& path) const
    {
        return OpenExactPath(MapFile(overrides, path));
    }

    std::vector<std::string> FileSystem::ReadLines(std::string path) const
    {
        return file_util::LinesIn(Open(path));
    }

    void FileSystem::SetOverrides(const std::map<std::string, std::string>& new_overrides)
    {
        overrides = new_overrides;
    }

    void FileSystem::ClearOverrides(const std::map<std::string, std::string>& new_overrides)
    {
        assert(overrides == new_overrides);
        overrides = {};
    }

    std::string FileSystem::MapFile(const std::map<std::string, std::string>& overrides, const std::string& path)
    {
        std::string finalpath = ToLower(path);

        auto found = overrides.find(finalpath);
        while (found != overrides.end())
        {
            finalpath = found->second;
            found = overrides.find(finalpath);
        }

        return finalpath;
    }
}
