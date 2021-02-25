#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "engine/entity.h"
#include "engine/renderable.h"
#include "engine/world.h"
#include "engine/xml.h"

namespace SimpleEngine
{
    struct MediaLoader;

    struct SimpleWorld : public World
    {
        std::vector<std::shared_ptr<Renderable>> worldRenderables;
        std::vector<std::shared_ptr<Renderable>> cameraRenderables;
        std::vector<std::shared_ptr<Entity>> entities;

        using RenderableAddTarget = std::function<void(std::shared_ptr<Renderable>)>;

        SimpleWorld(MediaLoader* loader, const std::string& file);

        void addEntities(MediaLoader* loader, std::shared_ptr<Xml::Element> level);

        void addMeshes(MediaLoader* loader, std::shared_ptr<Xml::Element> level, RenderableAddTarget target);

        void add(std::shared_ptr<Renderable> r) override;

        void remove(std::shared_ptr<Renderable> r) override;

        void worldSendTo(RenderList* list) override;

        void addCamera(std::shared_ptr<Renderable> r) override;

        void cameraSendTo(RenderList* list) override;

        void addEntity(std::shared_ptr<Entity> ent) override;

        void clearScreen() override;
    };
}
