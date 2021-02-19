﻿#pragma once

#include <memory>
#include <vector>

namespace SimpleEngine
{
    struct Renderable;
    struct MeshPart;
    struct vec3;
    struct quat;

    struct RenderDta
    {
        virtual ~RenderDta() = default;

        virtual int Id() = 0;
        virtual void render() = 0;
    };

    struct RenderList
    {
        std::vector<std::shared_ptr<RenderDta>> datas;

        void add(std::shared_ptr<MeshPart> part, const vec3& pos, const quat& rot);

        void add(std::shared_ptr<Renderable> r, int id);

        void render();
    };
}