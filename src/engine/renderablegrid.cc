using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Tao.OpenGl;
using System.Drawing;

namespace SimpleEngine
{
    struct Line
    {
        Line(vec3 s, vec3 e)
        {
            start = s;
            end = e;
        }
        vec3 start;
        vec3 end;

        void render()
        {
            Gl.glVertex3f(start.x, start.y, start.z);
            Gl.glVertex3f(end.x, end.y, end.z);
        }
    }

    struct RenderableGrid : Renderable
    {
        vec3 origin = vec3::Zero();
        vec3 xdir = vec3::Right();
        vec3 ydir = vec3::In();
        float halflength = 100;
        float spacing = 2;
        Color c = Color.LightGray;

        void sendToRenderer(RenderList r)
        {
            r.add(this, 0);
        }

        void render()
        {
            std::vector<Line> lines = std::vector<Line>();
            int count = (int)(halflength / spacing);
            for (int i = 0; i < count; ++i)
            {
                vec3 b = origin + ydir * spacing * (i + 1);
                lines.Add(Line(b - xdir * halflength, b + xdir * halflength));

                b = origin - ydir * spacing * i;
                lines.Add(Line(b - xdir * halflength, b + xdir * halflength));

                b = origin + xdir * spacing * (i + 1);
                lines.Add(Line(b - ydir * halflength, b + ydir * halflength));

                b = origin - xdir * spacing * i;
                lines.Add(Line(b - ydir * halflength, b + ydir * halflength));
            }

            Gl.glDisable(Gl.GL_TEXTURE_2D);
            Gl.glColor3f(c.R / 255.0f, c.G / 255.0f, c.B / 255.0f);
            Gl.glBegin(Gl.GL_LINES);
            for (Line l : lines)
                l.render();
            Gl.glEnd();
            Gl.glEnable(Gl.GL_TEXTURE_2D);
        }
    }
}
