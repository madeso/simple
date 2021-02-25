#pragma once

#include <string>

namespace SimpleEngine
{
    struct MediaLoader;
    struct FileSystem;

    struct Media
    {
        virtual ~Media() = default;
        virtual void Load(MediaLoader* ml, FileSystem* fs, const std::string& path) = 0;
    };
}
