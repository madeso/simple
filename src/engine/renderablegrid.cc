#include "engine/renderablegrid.h"

#include "engine/opengl.h"
#include "engine/renderlist.h"

namespace simple
{
    Line::Line(const vec3& s, const vec3& e)
        : start(s)
        , end(e)
    {
    }

    void Line::OnRender() const
    {
        glVertex3f(start.x, start.y, start.z);
        glVertex3f(end.x, end.y, end.z);
    }

    void RenderableGrid::SendToRenderer(RenderList* list)
    {
        list->Add(this, 0);
    }

    void RenderableGrid::OnRender()
    {
        auto lines = std::vector<Line>();
        const auto count = static_cast<int>(half_length / spacing);
        for (auto i = 0; i < count; ++i)
        {
            auto b = origin + second_direction * spacing * static_cast<float>(i + 1);
            lines.emplace_back(Line(b - first_direction * half_length, b + first_direction * half_length));

            b = origin - second_direction * spacing * i;
            lines.emplace_back(Line(b - first_direction * half_length, b + first_direction * half_length));

            b = origin + first_direction * spacing * (i + 1);
            lines.emplace_back(Line(b - second_direction * half_length, b + second_direction * half_length));

            b = origin - first_direction * spacing * i;
            lines.emplace_back(Line(b - second_direction * half_length, b + second_direction * half_length));
        }

        glDisable(GL_TEXTURE_2D);
        glColor3f(c.x, c.y, c.z);
        glBegin(GL_LINES);
        for (const auto& l : lines)
            l.OnRender();
        glEnd();
        glEnable(GL_TEXTURE_2D);
    }
}
