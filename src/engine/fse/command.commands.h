#pragma once

#include <memory>

#include "engine/xml.h"

namespace SimpleEngine::fse
{
    struct Command;
    struct Provider;
}

namespace SimpleEngine::fse::Commands
{
    std::shared_ptr<Command> Create(std::shared_ptr<Xml::Element> el, std::shared_ptr<Provider> pro);
}
