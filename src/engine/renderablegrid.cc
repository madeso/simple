#include "engine/renderablegrid.h"

#include "engine/opengl.h"
#include "engine/renderlist.h"

namespace SimpleEngine
{
    Line::Line(const vec3& s, const vec3& e)
        : start(s)
        , end(e)
    {
    }

    void Line::render() const
    {
        glVertex3f(start.x, start.y, start.z);
        glVertex3f(end.x, end.y, end.z);
    }

    void RenderableGrid::sendToRenderer(RenderList* r)
    {
        r->add(this, 0);
    }

    void RenderableGrid::render()
    {
        std::vector<Line> lines = std::vector<Line>();
        int count = (int)(halflength / spacing);
        for (int i = 0; i < count; ++i)
        {
            vec3 b = origin + ydir * spacing * (i + 1);
            lines.emplace_back(Line(b - xdir * halflength, b + xdir * halflength));

            b = origin - ydir * spacing * i;
            lines.emplace_back(Line(b - xdir * halflength, b + xdir * halflength));

            b = origin + xdir * spacing * (i + 1);
            lines.emplace_back(Line(b - ydir * halflength, b + ydir * halflength));

            b = origin - xdir * spacing * i;
            lines.emplace_back(Line(b - ydir * halflength, b + ydir * halflength));
        }

        glDisable(GL_TEXTURE_2D);
        glColor3f(c.x, c.y, c.z);
        glBegin(GL_LINES);
        for (Line l : lines)
            l.render();
        glEnd();
        glEnable(GL_TEXTURE_2D);
    }
}
