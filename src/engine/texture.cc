#include "engine/texture.h"

#include "engine/filesystem.h"
#include "engine/opengl.h"
#include "stb_image.h"

namespace simple
{
    Texture::Texture(bool alpha, int width, int height, unsigned char* bitmap_data, bool should_mipmap, int bitmap_format)
    {
        glGenTextures(1, &id);
        Bind();
        const int internal_format = alpha ? GL_RGBA8 : GL_RGB8;

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, should_mipmap ? GL_LINEAR_MIPMAP_NEAREST : GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, bitmap_format, GL_UNSIGNED_BYTE, bitmap_data);
        if (should_mipmap)
        {
            glGenerateMipmap(GL_TEXTURE_2D);
        }
    }

    void Texture::Bind(int location)
    {
        glActiveTexture(GL_TEXTURE0 + location);
        glBindTexture(GL_TEXTURE_2D, id);
    }

    Texture::~Texture()
    {
        glDeleteTextures(1, &id);
    }

    void TextureMedia::Load(MediaLoader* media_loader, FileSystem* file_system, const std::string& path)
    {
        const auto filename = file_system->Open(path);

        int width = 0;
        int height = 0;
        int channels = 0;
        stbi_set_flip_vertically_on_load(1);
        unsigned char* pixel_data = stbi_load(filename.c_str(), &width, &height, &channels, 3);

        if (pixel_data != nullptr)
        {
            const bool has_alpha = channels == 4;
            const int format = has_alpha ? GL_RGBA : GL_RGB;
            texture = std::make_shared<Texture>(has_alpha, width, height, pixel_data, false, format);
            stbi_image_free(pixel_data);
        }
    }

    void TextureMedia::Bind(int location)
    {
        texture->Bind(location);
    }
}
