#pragma once

#include <functional>

namespace SimpleEngine
{
    void RunMain(std::function<void()>&& runner);
}
