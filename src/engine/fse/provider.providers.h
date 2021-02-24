#pragma once

#include "engine/xml.h"

namespace SimpleEngine::fse
{
    struct Provider;
}

namespace SimpleEngine::fse::Providers
{
    std::shared_ptr<Provider> Create(std::shared_ptr<Xml::Element> x);
}
