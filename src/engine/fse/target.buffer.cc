#include "engine/fse/target.buffer.h"

#include "engine/fse/bufferreference.h"
#include "fmt/core.h"

namespace SimpleEngine::fse::Targets
{
    std::string BufferTarget::Name() const
    {
        return Id();
    }

    std::string BufferTarget::ToString() const
    {
        return fmt::format("{} targeting a buffer ({}x{}) named {}", Target::ToString(), width, height, Name());
    }

    BufferTarget::BufferTarget(std::shared_ptr<Xml::Element> el)
        : width(Xml::GetAttribute<int>(
              el, "width", [](const std::string& s) { return std::stoi(s); }, 512))
        , height(Xml::GetAttribute<int>(
              el, "height", [](const std::string& s) { return std::stoi(s); }, 512))
    {
    }

    void BufferTarget::apply(Target::ApplyFunction a)
    {
        buffer->updateTexture(a);
    }

    int BufferTarget::Width()
    {
        return buffer->Width();
    }

    int BufferTarget::Height()
    {
        return buffer->Height();
    }

    void BufferTarget::link(Linker*)
    {
        buffer = createBuffer(Name(), width, height);
    }
}
