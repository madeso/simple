#include "engine/fileutil.h"

#include <fstream>

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
    }
}
