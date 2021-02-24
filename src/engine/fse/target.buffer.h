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
        std::shared_ptr<BufferReference> buffer;
        int width;
        int height;

        std::string Name() const;

        std::string ToString() const override;

        BufferTarget(std::shared_ptr<Xml::Element> el);

        void apply(Target::ApplyFunction a) override;

        int Width() override;

        int Height() override;

        void link(Linker*) override;
    };
}
