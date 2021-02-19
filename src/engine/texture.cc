#include "engine/texture.h"

#include "engine/filesystem.h"
#include "engine/opengl.h"
#include "stb_image.h"

namespace SimpleEngine
{
    unsigned int Image::Id()
    {
        return text;
    }

    Image::Image(bool alpha, int width, int height, unsigned char* bitmapData, bool mipmap, int format)
    {
        glGenTextures(1, &text);
        bind();
        int internalformat = alpha ? GL_RGBA8 : GL_RGB8;

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mipmap ? GL_LINEAR_MIPMAP_NEAREST : GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, internalformat, width, height, 0, format, GL_UNSIGNED_BYTE, bitmapData);
        if (mipmap)
        {
            glGenerateMipmap(GL_TEXTURE_2D);
        }
    }

    void Image::bind(int location)
    {
        glActiveTexture(GL_TEXTURE0 + location);
        glBindTexture(GL_TEXTURE_2D, text);
    }

    Image::~Image()
    {
        glDeleteTextures(1, &text);
    }

    void Texture::load(MediaLoader* ml, FileSystem* fs, const std::string& path)
    {
        const auto filename = fs->open(path);

        int w = 0;
        int h = 0;
        int comp = 0;
        unsigned char* image = stbi_load(filename.c_str(), &w, &h, &comp, 3);

        if (image != nullptr)
        {
            const bool alpha = comp == 4;
            const int format = alpha ? GL_RGBA : GL_RGB;
            img = std::make_shared<Image>(alpha, w, h, image, false, format);
            stbi_image_free(image);
        }
    }

    void Texture::bind(int location)
    {
        img->bind(location);
    }
}
