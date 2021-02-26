#include "engine/fse/target.buffer.h"

#include "engine/fse/bufferreference.h"
#include "fmt/core.h"

namespace simple::fse::targets
{
    BufferTarget::BufferTarget(std::shared_ptr<xml::Element> el)
        : Target(el)
        , width(xml::GetAttribute<int>(
              el, "width", [](const std::string& s) { return std::stoi(s); }, 512))
        , height(xml::GetAttribute<int>(
              el, "height", [](const std::string& s) { return std::stoi(s); }, 512))
    {
    }

    std::string BufferTarget::GetName() const
    {
        return GetId();
    }

    std::string BufferTarget::ToString() const
    {
        return fmt::format("{} targeting a buffer ({}x{}) named {}", Target::ToString(), width, height, GetName());
    }

    void BufferTarget::Apply(Target::ApplyFunction a)
    {
        buffer->UpdateTexture(a);
    }

    int BufferTarget::GetWidth()
    {
        return buffer->GetWidth();
    }

    int BufferTarget::GetHeight()
    {
        return buffer->GetHeight();
    }

    void BufferTarget::Link(Linker*)
    {
        buffer = CreateBuffer(GetName(), width, height);
    }

    void BufferTarget::OnSize(int w, int h)
    {
    }
}
