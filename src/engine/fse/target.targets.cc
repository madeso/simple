#include "engine/fse/target.targets.h"

#include <stdexcept>

#include "engine/fse/target.buffer.h"
#include "engine/fse/target.form.h"
#include "fmt/format.h"

namespace simple::fse
{
    struct Target;
}

namespace simple::fse::targets
{
    std::shared_ptr<Target> Create(std::shared_ptr<xml::Element> x, int width, int height)
    {
        const auto name = x->GetName();

        if (name == "buffer")
        {
            return std::make_shared<BufferTarget>(x);
        }
        else if (name == "form")
        {
            return std::make_shared<FormTarget>(x, width, height);
        }
        else
        {
            throw std::runtime_error(fmt::format("{} is not a known target", name));
        }
    }
}
