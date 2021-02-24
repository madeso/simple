#pragma once

#include "engine/fse/target.h"

namespace SimpleEngine::fse::Targets
{
    struct FormTarget : public Target
    {
        int width;
        int height;

        FormTarget(int w, int h);

        std::string ToString() const override;

        void apply(Target::ApplyFunction a) override;

        int Width() override;

        int Height() override;

        void link(Linker*) override;
    };
}
