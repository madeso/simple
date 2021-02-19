#pragma once

#include <memory>

#include "engine/media.h"

namespace SimpleEngine
{
    struct Image
    {
        unsigned int text = 0;

        unsigned int Id();

        Image(bool alpha, int width, int height, unsigned char* bitmapData, bool mipmap, int format);

        void bind(int location = 0);

        ~Image();
    };

    struct Texture : public Media
    {
        std::shared_ptr<Image> img;

        void load(MediaLoader* ml, FileSystem* fs, const std::string& path) override;

        void bind(int location = 0);
    };
}
