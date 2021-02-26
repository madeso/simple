#include "engine/fse/bufferreference.h"

#include <stdexcept>

namespace simple::fse
{
    BufferReference::BufferReference(const std::string& name)
        : name(name)
    {
    }

    void BufferReference::SetBuffer(std::shared_ptr<Fbo> fbo)
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

    void BufferReference::BindTexture(int location)
    {
        buffer->BindTexture(location);
    }

    void BufferReference::UpdateTexture(std::function<void()> a)
    {
        buffer->UpdateTexture(std::move(a));
    }

    int BufferReference::GetWidth() const
    {
        return buffer->Width();
    }

    int BufferReference::GetHeight() const
    {
        return buffer->Height();
    }
}
