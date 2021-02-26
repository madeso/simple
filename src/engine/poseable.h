#pragma once

#include <memory>

namespace simple
{
    struct CompiledPose;

    struct Poseable
    {
        virtual ~Poseable() = default;
        virtual std::shared_ptr<CompiledPose> GetPose() = 0;
    };
}
