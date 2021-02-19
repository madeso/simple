#pragma once

#include <memory>

namespace SimpleEngine
{
    struct CompiledPose;

    struct Poseable
    {
        virtual ~Poseable() = default;
        virtual std::shared_ptr<CompiledPose> CurrentPose() = 0;
    };
}
