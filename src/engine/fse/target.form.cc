#include "engine/fse/target.form.h"

#include "fmt/core.h"

namespace SimpleEngine::fse::Targets
{
    FormTarget::FormTarget(std::shared_ptr<Xml::Element> x, int w, int h)
        : Target(x)
        , width(w)
        , height(h)
    {
    }

    std::string FormTarget::ToString() const
    {
        return fmt::format("{} targeting a form", Target::ToString());
    }

    void FormTarget::apply(Target::ApplyFunction a)
    {
        a();
    }

    int FormTarget::Width()
    {
        return width;
    }

    int FormTarget::Height()
    {
        return height;
    }

    void FormTarget::link(Linker*)
    {
    }
}
