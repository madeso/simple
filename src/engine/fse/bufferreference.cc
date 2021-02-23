#include "engine/fse/bufferreference.h"

#include <stdexcept>

namespace SimpleEngine::fse
{
    BufferReference::BufferReference(const std::string& name)
        : name(name)
    {
    }

    std::string BufferReference::Name() const
    {
        return name;
    }

    void BufferReference::setBuffer(std::shared_ptr<Fbo> fbo)
    {
        if (buffer == nullptr)
        {
            buffer = fbo;
        }
        else
        {
            throw std::runtime_error("Error, buffer already initialized!");
        }
    }

    void BufferReference::bindTexture(int location)
    {
        buffer->bindTexture(location);
    }

    void BufferReference::updateTexture(std::function<void()> a)
    {
        buffer->updateTexture(std::move(a));
    }

    int BufferReference::Width() const
    {
        return buffer->Width();
    }

    int BufferReference::Height() const
    {
        return buffer->Height();
    }
}
