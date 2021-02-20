#pragma once

#include <vector>

#include "engine/renderable.h"
#include "engine/vec3.h"

namespace SimpleEngine
{
    struct Line
    {
        Line(const vec3& s, const vec3& e);

        vec3 start;
        vec3 end;

        void render() const;
    };

    struct RenderableGrid : public Renderable
    {
        vec3 origin = vec3::Zero();
        vec3 xdir = vec3::Right();
        vec3 ydir = vec3::In();
        float halflength = 100;
        float spacing = 2;
        vec3 c = vec3{0.8f, 0.8f, 0.8f};  // light gray

        void sendToRenderer(RenderList* r) override;

        void render() override;
    };
}
