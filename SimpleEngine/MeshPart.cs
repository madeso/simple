using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Tao.OpenGl;

namespace SimpleEngine
{
    public class MeshPart
    {
        public List<MeshDef.Vertex[]> faces = new List<MeshDef.Vertex[]>();
        Material mat;
        Poseable poseable;

        public MeshPart(MediaLoader ml, MeshDef.MaterialDef m, MeshDef def, Poseable poseable)
        {
            this.poseable = poseable;

            mat = new Material(m.diffuse, ml.fetch<Texture>(m.texture));
            foreach (MeshDef.Tri tri in def.TrianglesFor(m))
            {
                faces.Add(def.lookup(tri));
            }
        }

        int id = -1;
        public int Id
        {
            get
            {
                if (id == -1) id = calculateId();
                return id;
            }
        }

        private int calculateId()
        {
            return 0;
        }
        internal void render(vec3 p, quat rot)
        {
            using(PushedMatrix dnum = new PushedMatrix() )
            {
                mat.apply();
                Gl.glTranslatef(p.x, p.y, p.z);
                AxisAngle aa = rot.AxisAngle;
                // apply rotation
                Gl.glRotatef(aa.angle.inDegrees, aa.axis.x, aa.axis.y, aa.axis.z);
                Gl.glBegin(Gl.GL_TRIANGLES);
                foreach (MeshDef.Vertex[] face in faces)
                {
                    for (int i = 0; i < 3; ++i)
                    {
                        MeshDef.Vertex v = face[i];
                        vec3 vert = (poseable.CurrentPose != null)
                            ? poseable.CurrentPose.transforms[v.bone] * v.vertex
                            : v.vertex;
                        Gl.glNormal3f(v.normal.x, v.normal.y, v.normal.z);
                        Gl.glTexCoord2f(v.uv.x, v.uv.y);
                        Gl.glVertex3f(vert.x, vert.y, vert.z);
                    }
                }
                Gl.glEnd();

            }
        }
    }
}
