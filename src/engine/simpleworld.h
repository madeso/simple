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
        std::vector<std::shared_ptr<Renderable>> world_renderables;
        std::vector<std::shared_ptr<Renderable>> camera_renderables;
        std::vector<std::shared_ptr<Entity>> entities;

        using RenderableAddTarget = std::function<void(std::shared_ptr<Renderable>)>;

        SimpleWorld(MediaLoader* loader, const std::string& file);

        void AddEntities(MediaLoader* loader, std::shared_ptr<Xml::Element> level);

        void AddMeshes(MediaLoader* loader, std::shared_ptr<Xml::Element> level, RenderableAddTarget target);

        void Add(std::shared_ptr<Renderable> r) override;

        void Remove(std::shared_ptr<Renderable> r) override;

        void SendWorldRenderablesToList(RenderList* list) override;

        void AddCameraRenderable(std::shared_ptr<Renderable> r) override;

        void SendCameraRenderablesToList(RenderList* list) override;

        void AddEntity(std::shared_ptr<Entity> ent) override;

        void ClearScreen() override;
    };
}
