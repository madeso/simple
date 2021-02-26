#pragma once

#include <functional>
#include <memory>
#include <string>

#include "engine/fbo.h"

namespace simple::fse
{
    struct BufferReference
    {
        std::shared_ptr<Fbo> buffer;
        std::string name;

        BufferReference(const std::string& name);

        void SetBuffer(std::shared_ptr<Fbo> fbo);

        void BindTexture(int location);

        void UpdateTexture(std::function<void()> a);

        int GetWidth() const;

        int GetHeight() const;
    };
}
