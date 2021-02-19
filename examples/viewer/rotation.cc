using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using SimpleEngine;
using Tao.OpenGl;
using System.Windows.Forms;

namespace ModelView
{
    struct Rotation
    {
        void sendMouse(vec2 mouse, vec2 oldmouse);
        void rotateGl();

        const float sens = 0.25f;
    }

    struct QuatRot : Rotation
    {
        quat rotation = quat::Identity();

        override void rotateGl()
        {
            AxisAngle aa = rotation.AxisAngle;
            Gl.glRotatef(aa.angle.inDegrees, aa.axis.x, aa.axis.y, aa.axis.z);
        }
    }

    struct EasyRotation : Rotation
    {
        float rx = 0;
        float ry = 0;

        override void sendMouse(vec2 mouse, vec2 oldmouse)
        {
            float dx = (mouse.x - oldmouse.x);
            float dy = -(mouse.y - oldmouse.y);
            float sens = 0.1f;
            rx += sens * dx;
            ry += sens * dy;
        }

        override void rotateGl()
        {
            Gl.glRotatef(-ry, 1, 0, 0);
            Gl.glRotatef(rx, 0, 1, 0);
        }
    }

    struct BasicQuatRot : QuatRot
    {
        override void sendMouse(vec2 current, vec2 oldmouse)
        {
            vec2 diff = vec2.FromTo(oldmouse, current);
            float dx = diff.x * sens;
            float dy = diff.y * sens;
            quat rx = quat(AxisAngle.RightHandAround(vec3::Up(), angle.FromDegrees(dx)));
            quat ry = quat(AxisAngle.RightHandAround(vec3::Right(), angle.FromDegrees(dy)));
            quat final = rx * ry;
            rotation = quat::Combine()(rotation, final);
        }
    } struct ArcBallRotation : QuatRot
    {
        Form ba;

        ArcBallRotation(Form owner)
        {
            this.ba = owner;
        }

        float Width
        {
            get
            {
                return ba.Width;
            }
        }
        float Height
        {
            get
            {
                return ba.Height;
            }
        }

        vec2 tranform(vec2 v)
        {
            return vec2(v.x, Height - v.y);
        }

        override void sendMouse(vec2 current, vec2 oldmouse)
        {
            ArcBall ball = ArcBall(vec2(Width / 2.0f, Height / 2.0f), Math.Min(Height, Width) / 2.0f);
            quat final = ball.rotation(tranform(oldmouse), tranform(current));

            rotation = quat::Combine()(rotation, final);
        }
    }
}
