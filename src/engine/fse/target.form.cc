#include "engine/fse/target.form.h"

#include "fmt/core.h"

namespace simple::fse::targets
{
    FormTarget::FormTarget(std::shared_ptr<xml::Element> x, int w, int h)
        : Target(x)
        , width(w)
        , height(h)
    {
    }

    std::string FormTarget::ToString() const
    {
        return fmt::format("{} targeting a form", Target::ToString());
    }

    void FormTarget::Apply(Target::ApplyFunction a)
    {
        a();
    }

    int FormTarget::GetWidth()
    {
        return width;
    }

    int FormTarget::GetHeight()
    {
        return height;
    }

    void FormTarget::Link(Linker*)
    {
    }

    void FormTarget::OnSize(int w, int h)
    {
        width = w;
        height = h;
    }
}
