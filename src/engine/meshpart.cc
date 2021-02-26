#include "engine/meshpart.h"

#include <memory>
#include <vector>

#include "engine/animation.h"
#include "engine/axisangle.h"
#include "engine/medialoader.h"
#include "engine/opengl.h"
#include "engine/pushedmatrix.h"
#include "engine/texture.h"

namespace simple
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

    void MeshPart::OnRender(const vec3& position, const quat& rotation) const
    {
        PushedMatrix pushed_matrix;

        material->Apply();
        glTranslatef(position.x, position.y, position.z);
        AxisAngle aa = rotation.GetAxisAngle();
        // apply rotation
        glRotatef(aa.angle.InDegrees(), aa.axis.x, aa.axis.y, aa.axis.z);
        glBegin(GL_TRIANGLES);
        for (const auto& face : faces)
        {
            for (auto i = 0; i < 3; ++i)
            {
                auto v = face[i];
                const auto vertex = (poseable->GetPose() != nullptr)
                                      ? poseable->GetPose()->transforms[v.bone] * v.position
                                      : v.position;
                glNormal3f(v.normal.x, v.normal.y, v.normal.z);
                glTexCoord2f(v.texture_coordinate.x, v.texture_coordinate.y);
                glVertex3f(vertex.x, vertex.y, vertex.z);
            }
        }
        glEnd();
    }
}
