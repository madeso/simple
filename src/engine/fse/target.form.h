#pragma once

#include "engine/fse/target.h"
#include "engine/xml.h"

namespace SimpleEngine::fse::Targets
{
    struct FormTarget : public Target
    {
        int width;
        int height;

        FormTarget(std::shared_ptr<Xml::Element> x, int w, int h);

        std::string ToString() const override;

        void apply(Target::ApplyFunction a) override;

        int Width() override;

        int Height() override;

        void link(Linker*) override;

        void OnSize(int width, int height) override;
    };
}
