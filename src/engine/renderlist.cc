#include "engine/renderlist.h"

#include <algorithm>
#include <memory>
#include <utility>

#include "engine/meshpart.h"
#include "engine/quat.h"
#include "engine/renderable.h"
#include "engine/vec3.h"

namespace simple
{
    struct RenderMeshPart : public RenderDta
    {
        std::shared_ptr<MeshPart> part;
        vec3 pos;
        quat rot;

        int GetId() override
        {
            return part->GetId();
        }

        void OnRender() override
        {
            part->OnRender(pos, rot);
        }
    };

    struct RenderRenderable : public RenderDta
    {
        int id;
        Renderable* r;

        int GetId() override
        {
            return id;
        }

        void OnRender() override
        {
            r->OnRender();
        }
    };

    bool Compare(std::shared_ptr<RenderDta> x, std::shared_ptr<RenderDta> y)
    {
        return x->GetId() < y->GetId();
    }

    void RenderList::Add(std::shared_ptr<MeshPart> part, const vec3& position, const quat& rotation)
    {
        auto rmp = std::make_shared<RenderMeshPart>();
        rmp->part = std::move(part);
        rmp->pos = position;
        rmp->rot = rotation;
        datas.emplace_back(rmp);
    }

    void RenderList::Add(Renderable* r, int id)
    {
        auto rr = std::make_shared<RenderRenderable>();
        rr->id = id;
        rr->r = r;
        datas.emplace_back(rr);
    }

    void RenderList::OnRender()
    {
        std::sort(datas.begin(), datas.end(), Compare);
        for (auto& data : datas)
        {
            data->OnRender();
        }
    }
}
