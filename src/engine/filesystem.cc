#include "engine/filesystem.h"

#include <cassert>
#include <filesystem>
#include <fstream>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

#include "engine/strings.h"

namespace SimpleEngine
{
    namespace fs = std::filesystem;

    FileSystem::FileSystem()
    {
    }

    void FileSystem::addRoot(const std::string& root)
    {
        roots.emplace_back(root);
    }

    void FileSystem::addDefaultRoots(const std::string& app, const std::string& game)
    {
        addRoot(fs::current_path().string());
        // addRoot(combine(Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData), app, game));
        // addRoot(combine(Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData), app, game));
        // addRoot(combine(Environment.GetFolderPath(Environment.SpecialFolder.CommonApplicationData), app, game));
        // addRoot(combine(Environment.GetFolderPath(Environment.SpecialFolder.ProgramFiles), app, game));
        // addRoot(combine(Environment.GetFolderPath(Environment.SpecialFolder.Programs), app, game));
    }

    std::string FileSystem::doOpen(const std::string& path) const
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
        throw std::runtime_error("Failed to open file " + path);
    }

    std::string FileSystem::open(const std::string& path) const
    {
        return doOpen(MapFile(overrides, path));
    }

    std::vector<std::string> FileSystem::readLines(std::string path) const
    {
        const auto p = open(path);
        std::ifstream infile(p.c_str());
        std::string line;
        std::vector<std::string> r;
        while (std::getline(infile, line))
        {
            r.emplace_back(line);
        }
        return r;
    }

    void FileSystem::setOverrides(const std::map<std::string, std::string>& new_overrides)
    {
        overrides = new_overrides;
    }

    void FileSystem::clearOverrides(const std::map<std::string, std::string>& new_overrides)
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
