#pragma once

#include <map>
#include <string>
#include <vector>

namespace SimpleEngine
{
    struct FileSystem
    {
        std::vector<std::string> roots;
        std::map<std::string, std::string> overrides;

        FileSystem();

        void addRoot(const std::string& root);

        void addDefaultRoots(const std::string& app, const std::string& game);

        std::string doOpen(const std::string& path) const;

        std::string open(const std::string& path) const;

        std::vector<std::string> readLines(std::string path) const;

        void setOverrides(const std::map<std::string, std::string>& new_overrides);

        void clearOverrides(const std::map<std::string, std::string>& new_overrides);

        static std::string MapFile(const std::map<std::string, std::string>& overrides, const std::string& path);
    };
}
