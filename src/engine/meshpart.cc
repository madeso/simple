#include <memory>
#include <vector>

#include "engine/material.h"
#include "engine/medialoader.h"
#include "engine/meshdef.h"
#include "engine/opengl.h"
#include "engine/poseable.h"
#include "engine/texture.h"

namespace SimpleEngine
{
    struct Poseable;

    struct MeshPart
    {
        std::vector<std::vector<Vertex>> faces;
        std::shared_ptr<Material> mat;
        std::shared_ptr<Poseable> poseable;
        int id = -1;

        MeshPart(MediaLoader* ml, const MaterialDef& m, MeshDef* def, std::shared_ptr<Poseable> p)
            : poseable(p)
        {
            mat = Material(m.diffuse, ml->fetch<Texture>(m.texture));
            for (Tri tri : def->TrianglesFor(m))
            {
                faces.emplace_back(def->lookup(tri));
            }
        }

        int Id()
        {
            if (id == -1)
                id = calculateId();
            return id;
        }

        int calculateId() const
        {
            return 0;
        }

        void render(vec3 p, quat rot)
        {
            {
                PushedMatrix dnum;

                mat->apply();
                glTranslatef(p.x, p.y, p.z);
                AxisAngle aa = rot.AxisAngle();
                // apply rotation
                glRotatef(aa.angle.inDegrees, aa.axis.x, aa.axis.y, aa.axis.z);
                glBegin(GL_TRIANGLES);
                for (auto& face : faces)
                {
                    for (int i = 0; i < 3; ++i)
                    {
                        Vertex v = face[i];
                        vec3 vert = (poseable->CurrentPose() != nullptr)
                                        ? poseable->CurrentPose().transforms[v.bone] * v.vertex
                                        : v.vertex;
                        glNormal3f(v.normal.x, v.normal.y, v.normal.z);
                        glTexCoord2f(v.uv.x, v.uv.y);
                        glVertex3f(vert.x, vert.y, vert.z);
                    }
                }
                glEnd();
            }
        }
    }
}
