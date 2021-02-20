#pragma once

#include <string>
#include <vector>

namespace SimpleEngine
{
    struct BinaryReader
    {
        BinaryReader(const std::string& p);

        int ReadInt32();
        float ReadSingle();
        std::string ReadString();
    };

    struct BinaryWriter
    {
        BinaryWriter(const std::string& p);

        void WriteInt32(int i);
        void WriteSingle(float f);
        void WriteString(const std::string& str);
    };

    namespace FileUtil
    {
        std::vector<std::string> LinesIn(const std::string& path);

        std::pair<std::string, std::string> Split(const std::string& path);
        std::string GetExtension(const std::string& p);
        std::string GetFileNameWithoutExtension(const std::string& p);
    }
}
