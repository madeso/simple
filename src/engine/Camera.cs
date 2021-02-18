using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Tao.OpenGl;

namespace SimpleEngine
{
    public class Camera
    {
        internal void sendRotationAndPosition()
        {
            sendRotation();
            sendPosition();
        }

        internal void sendPosition()
        {
            // -position
            Gl.glTranslatef(-location.x, -location.y, -location.z);
        }

        internal void sendRotation()
        {
            // -rotatation
            AxisAngle aa = rotation.AxisAngle;
            Gl.glRotatef(-aa.angle.inDegrees, aa.axis.x, aa.axis.y, aa.axis.z);
        }

        public void rotate(quat q)
        {
            rotation = quat.Combine(rotation, q);
        }

        public vec3 location = vec3.Zero;
        public quat rotation = quat.Identity;
    }
}
