#pragma once

#include <map>
#include <string>
#include <vector>

namespace simple
{
    struct FileSystem
    {
        std::vector<std::string> roots;
        std::map<std::string, std::string> overrides;

        FileSystem();

        void AddRoot(const std::string& root);

        void AddDefaultRoots(const std::string& app, const std::string& game);

        std::string OpenExactPath(const std::string& path) const;

        std::string Open(const std::string& path) const;

        std::vector<std::string> ReadLines(std::string path) const;

        void SetOverrides(const std::map<std::string, std::string>& new_overrides);

        void ClearOverrides(const std::map<std::string, std::string>& new_overrides);

        static std::string MapFile(const std::map<std::string, std::string>& overrides, const std::string& path);
    };
}
