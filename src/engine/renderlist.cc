using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimpleEngine
{
    struct RenderList
    {
        struct RenderDta
        {
            int Id
            {
                get;
            }

            void render();
        }

        struct RenderMeshPart : RenderDta
        {
            MeshPart part;
            vec3 pos;
            quat rot;

            override int Id
            {
                get { return part.Id; }
            }

            override void render()
            {
                part.render(pos, rot);
            }
        }

        struct RenderRenderable : RenderDta
        {
            int id;
            Renderable r;

            override int Id
            {
                get { return id; }
            }

            override void render()
            {
                r.render();
            }
        }

        std::vector<RenderDta>
            datas = std::vector<RenderDta>();

        struct RenderDataComparer : Comparer<RenderDta>
        {
            override int Compare(RenderDta x, RenderDta y)
            {
                if (x.Id == y.Id)
                    return 0;
                else if (x.Id < y.Id)
                    return -1;
                else
                    return 1;
            }
        }

        void
        add(MeshPart part, vec3 pos, quat rot)
        {
            RenderMeshPart d = RenderMeshPart();
            d.part = part;
            d.pos = pos;
            d.rot = rot;
            datas.Add(d);
        }

        void add(Renderable r, int id)
        {
            RenderRenderable rr = RenderRenderable();
            rr.id = id;
            rr.r = r;
            datas.Add(rr);
        }

        void render()
        {
            datas.Sort(RenderDataComparer());
            for (RenderDta data : datas)
            {
                data.render();
            }
        }
    }
}
