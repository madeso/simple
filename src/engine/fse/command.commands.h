#pragma once

#include <memory>

#include "engine/xml.h"

namespace simple::fse
{
    struct Command;
    struct Provider;
}

namespace simple::fse::commands
{
    std::shared_ptr<Command> Create(std::shared_ptr<xml::Element> el, std::shared_ptr<Provider> pro);
}
