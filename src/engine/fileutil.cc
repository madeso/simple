#include "engine/fileutil.h"

#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

namespace SimpleEngine
{
    namespace FileUtil
    {
        std::vector<std::string> LinesIn(const std::string& path)
        {
            std::ifstream infile(path.c_str());
            std::string line;
            std::vector<std::string> r;
            while (std::getline(infile, line))
            {
                r.emplace_back(line);
            }
            return r;
        }

        std::pair<std::string, std::string> Split(const std::string& path)
        {
            auto p = fs::path{path};
            return {p.parent_path(), p.filename()};
        }

        std::string GetExtension(const std::string& p)
        {
            return fs::path{p}.extension();
        }
    }
}
