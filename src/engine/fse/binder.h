#pragma once

#include <memory>
#include <string>

#include "engine/fse/bufferreference.h"
#include "engine/fse/map.h"
#include "engine/pool.h"
#include "engine/shader.h"

namespace simple::fse
{
    struct FboCreator
    {
        Pool<std::shared_ptr<Fbo>> pool;
        Map<std::shared_ptr<Fbo>> map;

        FboCreator(int width, int height);

        std::shared_ptr<Fbo> create(const std::string& name);

        void Destroy(std::shared_ptr<Fbo> fbo);
    };

    struct Binder
    {
        Map<std::shared_ptr<ShaderProgram>> shaders;
        std::vector<std::shared_ptr<BufferReference>> references;
        std::map<std::string, Size> associations;
        std::map<Size, std::shared_ptr<FboCreator>> creators;

        std::string ToString() const;

        Binder(std::shared_ptr<FileSystem> sys);

        std::shared_ptr<ShaderProgram> GetShader(const std::string& shadername);

        std::shared_ptr<ShaderProgram> GetShaderOrNull(const std::string& shadername);

        void AddReference(std::shared_ptr<BufferReference> br);

        //Pool<Fbo> cbs = Pool<Fbo>(delegate() { return Fbo(512, 512, false); });
        //Map<Fbo> buffers = Map<Fbo>(delegate(std::string n) { return allocate(n); });

        std::shared_ptr<Fbo> CreateFbo(const std::string& name);

        std::shared_ptr<FboCreator> GetCreator(const std::string& name);

        void DestroyFbo(std::shared_ptr<Fbo> buff);

        Size GetSize(const std::string& name);

        void CreateBuffers();

        void SetSize(std::string p, Size size);
    };
}
