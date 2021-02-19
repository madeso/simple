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

    MeshPart::MeshPart(MediaLoader* ml, const MaterialDef& m, MeshDef* def, Poseable* p)
        : poseable(p)
    {
        mat = std::make_shared<Material>(m.diffuse, ml->fetch<Texture>(m.texture));
        for (const auto& tri : def->TrianglesFor(m))
        {
            faces.emplace_back(def->lookup(tri));
        }
    }

    int MeshPart::Id()
    {
        if (id == -1)
            id = calculateId();
        return id;
    }

    int MeshPart::calculateId() const
    {
        return 0;
    }

    void MeshPart::render(const vec3& p, const quat& rot) const
    {
        PushedMatrix dnum;

        mat->apply();
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
                vec3 vert = (poseable->CurrentPose() != nullptr)
                                ? poseable->CurrentPose()->transforms[v.bone] * v.vertex
                                : v.vertex;
                glNormal3f(v.normal.x, v.normal.y, v.normal.z);
                glTexCoord2f(v.uv.x, v.uv.y);
                glVertex3f(vert.x, vert.y, vert.z);
            }
        }
        glEnd();
    }
}
