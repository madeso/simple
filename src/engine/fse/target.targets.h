#pragma once

#include <memory>

#include "engine/xml.h"

namespace simple::fse
{
    struct Target;
}

namespace simple::fse::targets
{
    std::shared_ptr<Target> Create(std::shared_ptr<xml::Element> x, int width, int height);
}
