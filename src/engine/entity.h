#pragma once

#include <memory>
#include <string>
#include <vector>

#include "engine/renderable.h"
#include "engine/xml.h"

namespace SimpleEngine
{
    struct MediaLoader;
    struct vec3;
    struct quat;

    struct Entity
    {
        std::vector<std::shared_ptr<Renderable>> renderables;
        std::string name;

        Entity(const std::string& n);

        std::string ToString() const;

        static std::shared_ptr<Entity> Create(MediaLoader* loader, const std::string& name, std::shared_ptr<Xml::Element> root, const vec3& pos, const quat& rot);

        void AddRenderable(std::shared_ptr<Renderable> r);
    };
}
