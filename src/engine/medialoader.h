#pragma once

#include <map>
#include <memory>
#include <string>

#include "engine/media.h"

namespace SimpleEngine
{
    struct FileSystem;
    struct Media;

    struct MediaLoader
    {
        std::shared_ptr<FileSystem> sys;
        std::map<std::string, std::weak_ptr<Media>> loaded;

        std::shared_ptr<FileSystem> FS();
        MediaLoader(std::shared_ptr<FileSystem> fs);

        template <typename T>
        std::shared_ptr<T> fetch(const std::string& file)
        {
            auto found = loaded.find(file);
            if (found != loaded.end())
            {
                auto locked = found->second.lock();
                if (locked != nullptr)
                {
                    return std::static_pointer_cast<T>(locked);
                }
            }

            return forceLoad<T>(file);
        }

        template <typename T>
        std::shared_ptr<T> forceLoad(const std::string& file)
        {
            auto t = std::make_shared<T>();
            t->load(this, sys.get(), file);
            loaded[file] = t;
            return t;
        }
    };
}
