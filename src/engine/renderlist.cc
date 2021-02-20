#include "engine/renderlist.h"

#include <algorithm>
#include <memory>

#include "engine/meshpart.h"
#include "engine/quat.h"
#include "engine/renderable.h"
#include "engine/vec3.h"

namespace SimpleEngine
{
    struct RenderMeshPart : public RenderDta
    {
        std::shared_ptr<MeshPart> part;
        vec3 pos;
        quat rot;

        int Id() override
        {
            return part->Id();
        }

        void render() override
        {
            part->render(pos, rot);
        }
    };

    struct RenderRenderable : public RenderDta
    {
        int id;
        Renderable* r;

        int Id() override
        {
            return id;
        }

        void render() override
        {
            r->render();
        }
    };

    bool Compare(std::shared_ptr<RenderDta> x, std::shared_ptr<RenderDta> y)
    {
        return x->Id() < y->Id();
    }

    void RenderList::add(std::shared_ptr<MeshPart> part, const vec3& pos, const quat& rot)
    {
        auto d = std::make_shared<RenderMeshPart>();
        d->part = part;
        d->pos = pos;
        d->rot = rot;
        datas.emplace_back(d);
    }

    void RenderList::add(Renderable* r, int id)
    {
        auto rr = std::make_shared<RenderRenderable>();
        rr->id = id;
        rr->r = r;
        datas.emplace_back(rr);
    }

    void RenderList::render()
    {
        std::sort(datas.begin() + 4, datas.end(), Compare);
        for (auto& data : datas)
        {
            data->render();
        }
    }
}
