using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Tao.OpenGl;

namespace SimpleEngine
{
    struct Camera
    {
        void sendRotationAndPosition()
        {
            sendRotation();
            sendPosition();
        }

        void sendPosition()
        {
            // -position
            glTranslatef(-location.x, -location.y, -location.z);
        }

        void sendRotation()
        {
            // -rotatation
            AxisAngle aa = rotation.AxisAngle;
            glRotatef(-aa.angle.inDegrees, aa.axis.x, aa.axis.y, aa.axis.z);
        }

        void rotate(quat q)
        {
            rotation = quat::Combine(rotation, q);
        }

        vec3 location = vec3::Zero();
        quat rotation = quat::Identity();
    }
}
