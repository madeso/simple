#pragma once

#include <memory>
#include <string>

#include "engine/fse/target.h"
#include "engine/xml.h"

namespace simple::fse
{
    struct BufferReference;
}

namespace simple::fse::targets
{
    struct BufferTarget : public Target
    {
        BufferTarget(std::shared_ptr<xml::Element> el);

        std::shared_ptr<BufferReference> buffer;
        int width;
        int height;

        std::string GetName() const;

        std::string ToString() const override;

        void Apply(Target::ApplyFunction a) override;

        int GetWidth() override;

        int GetHeight() override;

        void Link(Linker*) override;

        void OnSize(int w, int h) override;
    };
}
