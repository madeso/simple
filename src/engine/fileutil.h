﻿#pragma once

#include <fstream>
#include <string>
#include <vector>

namespace simple
{
    struct BinaryReader
    {
        std::ifstream handle;

        BinaryReader(const std::string& p);

        int ReadInt32();
        float ReadSingle();
        std::string ReadString();
    };

    struct BinaryWriter
    {
        std::ofstream handle;
        BinaryWriter(const std::string& p);

        void WriteInt32(int i);
        void WriteSingle(float f);
        void WriteString(const std::string& str);
    };

    namespace file_util
    {
        std::vector<std::string> LinesIn(const std::string& path);

        std::pair<std::string, std::string> Split(const std::string& path);
        std::string GetExtension(const std::string& p);
        std::string GetFileNameWithoutExtension(const std::string& p);
        std::string GetFileName(const std::string& p);
    }
}
