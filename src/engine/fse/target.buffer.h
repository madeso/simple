#pragma once

#include <memory>
#include <string>

#include "engine/fse/target.h"
#include "engine/xml.h"

namespace SimpleEngine::fse
{
    struct BufferReference;
}

namespace SimpleEngine::fse::Targets
{
    struct BufferTarget : public Target
    {
        BufferTarget(std::shared_ptr<Xml::Element> el);

        std::shared_ptr<BufferReference> buffer;
        int width;
        int height;

        std::string Name() const;

        std::string ToString() const override;

        void apply(Target::ApplyFunction a) override;

        int Width() override;

        int Height() override;

        void link(Linker*) override;
    };
}
