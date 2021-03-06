#pragma once

#include <memory>
#include <vector>

namespace simple
{
    struct Renderable;
    struct MeshPart;
    struct vec3;
    struct quat;

    struct RenderDta
    {
        virtual ~RenderDta() = default;

        virtual int GetId() = 0;
        virtual void OnRender() = 0;
    };

    struct RenderList
    {
        std::vector<std::shared_ptr<RenderDta>> datas;

        void Add(std::shared_ptr<MeshPart> part, const vec3& position, const quat& rotation);

        void Add(Renderable* r, int id);

        void OnRender();
    };
}
