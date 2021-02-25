#pragma once

#include <memory>
#include <string>

#include "engine/fse/bufferreference.h"
#include "engine/fse/map.h"
#include "engine/pool.h"
#include "engine/shader.h"

namespace SimpleEngine::fse
{
    struct FboCreator
    {
        Pool<std::shared_ptr<Fbo>> pool;
        Map<std::shared_ptr<Fbo>> map;

        FboCreator(int width, int height);

        std::shared_ptr<Fbo> create(const std::string& name);

        void release(std::shared_ptr<Fbo> fbo);
    };

    struct Binder
    {
        Map<std::shared_ptr<Shader>> shaders;
        std::vector<std::shared_ptr<BufferReference>> references;
        std::map<std::string, Size> associations;
        std::map<Size, std::shared_ptr<FboCreator>> creators;

        std::string ToString() const;

        Binder(std::shared_ptr<FileSystem> sys);

        std::shared_ptr<Shader> getShader(const std::string& shadername);

        std::shared_ptr<Shader> getShaderOrNull(const std::string& shadername);

        void reference(std::shared_ptr<BufferReference> br);

        //Pool<Fbo> cbs = Pool<Fbo>(delegate() { return Fbo(512, 512, false); });
        //Map<Fbo> buffers = Map<Fbo>(delegate(std::string n) { return allocate(n); });

        std::shared_ptr<Fbo> allocate(const std::string& name);

        std::shared_ptr<FboCreator> getCreator(const std::string& name);

        void release(std::shared_ptr<Fbo> buff);

        Size sizeOf(const std::string& name);

        void createBuffers();

        void associate(std::string p, Size size);
    };
}
