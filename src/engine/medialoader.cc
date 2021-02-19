#include "engine/medialoader.h"

namespace SimpleEngine
{
    std::shared_ptr<FileSystem> MediaLoader::FS()
    {
        return sys;
    }

    MediaLoader::MediaLoader(std::shared_ptr<FileSystem> fs)
        : sys(fs)
    {
    }
}
