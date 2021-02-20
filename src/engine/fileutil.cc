#include "engine/fileutil.h"

#include <filesystem>

namespace fs = std::filesystem;

namespace SimpleEngine
{
    BinaryReader::BinaryReader(const std::string& path)
        : handle(path.c_str(), std::ios::binary)
    {
    }

    int BinaryReader::ReadInt32()
    {
        int i;
        handle >> i;
        return i;
    }

    float BinaryReader::ReadSingle()
    {
        float i;
        handle >> i;
        return i;
    }

    std::string BinaryReader::ReadString()
    {
        std::string str;
        std::getline(handle, str, static_cast<char>(0));
        return str;
    }

    BinaryWriter::BinaryWriter(const std::string& path)
        : handle(path.c_str(), std::ios::binary)
    {
    }

    void BinaryWriter::WriteInt32(int i)
    {
        handle << i;
    }

    void BinaryWriter::WriteSingle(float f)
    {
        handle << f;
    }

    void BinaryWriter::WriteString(const std::string& str)
    {
        handle << str;
        handle << static_cast<char>(0);
    }

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
