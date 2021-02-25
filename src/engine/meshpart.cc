#include "engine/meshpart.h"

#include <memory>
#include <vector>

#include "engine/animation.h"
#include "engine/axisangle.h"
#include "engine/medialoader.h"
#include "engine/opengl.h"
#include "engine/pushedmatrix.h"
#include "engine/texture.h"

namespace SimpleEngine
{
    struct Poseable;

    MeshPart::MeshPart(MediaLoader* ml, const MaterialDefinition& m, MeshDef* def, Poseable* p)
        : poseable(p)
    {
        material = std::make_shared<Material>(m.diffuse, ml->GetOrLoad<TextureMedia>(m.texture));
        for (const auto& tri : def->GetTrianglesFor(m))
        {
            faces.emplace_back(def->GetVerticesForTriangle(tri));
        }
    }

    int MeshPart::GetId()
    {
        if (id == -1)
            id = CalculateId();
        return id;
    }

    int MeshPart::CalculateId() const
    {
        return 0;
    }

    void MeshPart::OnRender(const vec3& p, const quat& rot) const
    {
        PushedMatrix dnum;

        material->Apply();
        glTranslatef(p.x, p.y, p.z);
        AxisAngle aa = rot.AxisAngle();
        // apply rotation
        glRotatef(aa.angle.inDegrees(), aa.axis.x, aa.axis.y, aa.axis.z);
        glBegin(GL_TRIANGLES);
        for (auto& face : faces)
        {
            for (int i = 0; i < 3; ++i)
            {
                Vertex v = face[i];
                vec3 vert = (poseable->GetPose() != nullptr)
                                ? poseable->GetPose()->transforms[v.bone] * v.vertex
                                : v.vertex;
                glNormal3f(v.normal.x, v.normal.y, v.normal.z);
                glTexCoord2f(v.uv.x, v.uv.y);
                glVertex3f(vert.x, vert.y, vert.z);
            }
        }
        glEnd();
    }
}
