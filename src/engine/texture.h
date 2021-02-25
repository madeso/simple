#pragma once

#include <memory>

#include "engine/media.h"

namespace SimpleEngine
{
    struct Texture
    {
        unsigned int texture_id = 0;

        unsigned int GetId();

        Texture(bool alpha, int width, int height, unsigned char* bitmap_data, bool mipmap, int format);

        void Bind(int location = 0);

        ~Texture();
    };

    struct TextureMedia : public Media
    {
        std::shared_ptr<Texture> image;

        void Load(MediaLoader* ml, FileSystem* fs, const std::string& path) override;

        void Bind(int location = 0);
    };
}
