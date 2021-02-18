using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimpleEngine
{
    public class RenderList
    {
        abstract class RenderDta
        {
            public abstract int Id
            {
                get;
            }

            public abstract void render();
        }

        class RenderMeshPart : RenderDta
        {
            public MeshPart part;
            public vec3 pos;
            public quat rot;

            public override int Id
            {
                get { return part.Id; }
            }

            public override void render()
            {
                part.render(pos, rot);
            }
        }

        class RenderRenderable : RenderDta
        {
            public int id;
            public Renderable r;

            public override int Id
            {
                get { return id; }
            }

            public override void render()
            {
                r.render();
            }
        }

        private List<RenderDta> datas = new List<RenderDta>();

        class RenderDataComparer : Comparer<RenderDta>
        {
            public override int Compare(RenderDta x, RenderDta y)
            {
                if (x.Id == y.Id) return 0;
                else if (x.Id < y.Id) return -1;
                else return 1;
            }
        }

        public void add(MeshPart part, vec3 pos, quat rot)
        {
            RenderMeshPart d = new RenderMeshPart();
            d.part = part;
            d.pos = pos;
            d.rot = rot;
            datas.Add(d);
        }

        public void add(Renderable r, int id)
        {
            RenderRenderable rr = new RenderRenderable();
            rr.id = id;
            rr.r = r;
            datas.Add(rr);
        }

        public void render()
        {
            datas.Sort(new RenderDataComparer());
            foreach (RenderDta data in datas)
            {
                data.render();
            }
        }
    }
}
