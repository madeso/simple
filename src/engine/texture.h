#pragma once

#include <memory>

#include "engine/media.h"

namespace simple
{
    struct Texture
    {
        unsigned int id = 0;

        Texture(bool alpha, int width, int height, unsigned char* bitmap_data, bool should_mipmap, int bitmap_format);

        void Bind(int location = 0);

        ~Texture();
    };

    struct TextureMedia : public Media
    {
        std::shared_ptr<Texture> texture;

        void Load(MediaLoader* media_loader, FileSystem* file_system, const std::string& path) override;

        void Bind(int location = 0);
    };
}
