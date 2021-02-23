#pragma once

#include <memory>
#include <string>

namespace SimpleEngine
{
    struct Renderable;
    struct Entity;
    struct RenderList;
    struct MediaLoader;
    struct Camera;

    struct World
    {
        virtual void add(std::shared_ptr<Renderable> r) = 0;
        virtual void addCamera(std::shared_ptr<Renderable> r) = 0;
        virtual void remove(std::shared_ptr<Renderable> r) = 0;
        virtual void worldSendTo(RenderList* list) = 0;
        virtual void cameraSendTo(RenderList* list) = 0;
        virtual void addEntity(std::shared_ptr<Entity> ent) = 0;

        static std::shared_ptr<World> Load(MediaLoader* loader, const std::string& file);
        void render(int width, int height, const Camera& c);
    };
}
