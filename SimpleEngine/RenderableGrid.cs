using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Tao.OpenGl;
using System.Drawing;

namespace SimpleEngine
{

    class Line
    {
        public Line(vec3 s, vec3 e)
        {
            start = s;
            end = e;
        }
        readonly vec3 start;
        readonly vec3 end;

        public void render()
        {
            Gl.glVertex3f(start.x, start.y, start.z);
            Gl.glVertex3f(end.x, end.y, end.z);
        }
    }

    public class RenderableGrid : Renderable
    {
        public vec3 origin = vec3.Zero;
        public vec3 xdir = vec3.Right;
        public vec3 ydir = vec3.In;
        public float halflength = 100;
        public float spacing = 2;
        public Color c = Color.LightGray;

        public void sendToRenderer(RenderList r)
        {
            r.add(this, 0);
        }

        public void render()
        {
            List<Line> lines = new List<Line>();
            int count = (int)(halflength / spacing);
            for (int i = 0; i < count; ++i)
            {
                vec3 b = origin + ydir * spacing * (i+1);
                lines.Add( new Line( b - xdir*halflength,  b + xdir*halflength) );

                b = origin - ydir * spacing * i;
                lines.Add(new Line(b - xdir * halflength, b + xdir * halflength));

                b = origin + xdir * spacing * (i + 1);
                lines.Add(new Line(b - ydir * halflength, b + ydir * halflength));

                b = origin - xdir * spacing * i;
                lines.Add(new Line(b - ydir * halflength, b + ydir * halflength));
            }

            Gl.glDisable(Gl.GL_TEXTURE_2D);
            Gl.glColor3f(c.R/255.0f, c.G/255.0f, c.B/255.0f);
            Gl.glBegin(Gl.GL_LINES);
            foreach (Line l in lines) l.render();
            Gl.glEnd();
            Gl.glEnable(Gl.GL_TEXTURE_2D);
        }
    }
}
