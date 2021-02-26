#pragma once

#include <memory>
#include <string>

namespace simple
{
    struct Renderable;
    struct Entity;
    struct RenderList;
    struct MediaLoader;
    struct Camera;

    struct World
    {
        virtual void Add(std::shared_ptr<Renderable> r) = 0;
        virtual void AddCameraRenderable(std::shared_ptr<Renderable> r) = 0;
        virtual void Remove(std::shared_ptr<Renderable> r) = 0;
        virtual void SendWorldRenderablesToList(RenderList* list) = 0;
        virtual void SendCameraRenderablesToList(RenderList* list) = 0;
        virtual void AddEntity(std::shared_ptr<Entity> ent) = 0;
        virtual void ClearScreen() = 0;

        static std::shared_ptr<World> Load(MediaLoader* loader, const std::string& file);
        void Render(int width, int height, const Camera& camera);
    };
}
