#pragma once

#include "engine/xml.h"

namespace simple::fse
{
    struct Provider;
}

namespace simple::fse::providers
{
    std::shared_ptr<Provider> Create(std::shared_ptr<xml::Element> x);
}
