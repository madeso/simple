#include "engine/texture.h"

#include "engine/filesystem.h"
#include "engine/opengl.h"
#include "stb_image.h"

namespace SimpleEngine
{
    unsigned int Texture::GetId()
    {
        return texture_id;
    }

    Texture::Texture(bool alpha, int width, int height, unsigned char* bitmap_data, bool mipmap, int format)
    {
        glGenTextures(1, &texture_id);
        Bind();
        int internal_format = alpha ? GL_RGBA8 : GL_RGB8;

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mipmap ? GL_LINEAR_MIPMAP_NEAREST : GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format, GL_UNSIGNED_BYTE, bitmap_data);
        if (mipmap)
        {
            glGenerateMipmap(GL_TEXTURE_2D);
        }
    }

    void Texture::Bind(int location)
    {
        glActiveTexture(GL_TEXTURE0 + location);
        glBindTexture(GL_TEXTURE_2D, texture_id);
    }

    Texture::~Texture()
    {
        glDeleteTextures(1, &texture_id);
    }

    void TextureMedia::Load(MediaLoader* ml, FileSystem* fs, const std::string& path)
    {
        const auto filename = fs->open(path);

        int w = 0;
        int h = 0;
        int comp = 0;
        stbi_set_flip_vertically_on_load(1);
        unsigned char* pixel_data = stbi_load(filename.c_str(), &w, &h, &comp, 3);

        if (pixel_data != nullptr)
        {
            const bool alpha = comp == 4;
            const int format = alpha ? GL_RGBA : GL_RGB;
            image = std::make_shared<Texture>(alpha, w, h, pixel_data, false, format);
            stbi_image_free(pixel_data);
        }
    }

    void TextureMedia::Bind(int location)
    {
        image->Bind(location);
    }
}
