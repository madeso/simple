#pragma once

#include <functional>
#include <memory>
#include <string>

#include "engine/fbo.h"

namespace SimpleEngine::fse
{
    struct BufferReference
    {
        std::shared_ptr<Fbo> buffer;
        std::string name;

        BufferReference(const std::string& name);

        std::string Name() const;

        void setBuffer(std::shared_ptr<Fbo> fbo);

        void bindTexture(int location);

        void updateTexture(std::function<void()> a);

        int Width() const;

        int Height() const;
    };
}
