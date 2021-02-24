#pragma once

#include <memory>

#include "engine/xml.h"

namespace SimpleEngine::fse
{
    struct Target;
}

namespace SimpleEngine::fse::Targets
{
    std::shared_ptr<Target> Create(std::shared_ptr<Xml::Element> x, int width, int height);
}
