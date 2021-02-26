#pragma once

#include "engine/fse/target.h"
#include "engine/xml.h"

namespace simple::fse::targets
{
    struct FormTarget : public Target
    {
        int width;
        int height;

        FormTarget(std::shared_ptr<xml::Element> x, int w, int h);

        std::string ToString() const override;

        void Apply(Target::ApplyFunction a) override;

        int GetWidth() override;

        int GetHeight() override;

        void Link(Linker*) override;

        void OnSize(int width, int height) override;
    };
}
