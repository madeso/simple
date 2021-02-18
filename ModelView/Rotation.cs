using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using SimpleEngine;
using Tao.OpenGl;
using System.Windows.Forms;

namespace ModelView
{
    public abstract class Rotation
    {
        public abstract void sendMouse(vec2 mouse, vec2 oldmouse);
        public abstract void rotateGl();

        protected const float sens = 0.25f;
    }

    public abstract class QuatRot : Rotation
    {
        protected quat rotation = quat.Identity;

        public override void rotateGl()
        {
            AxisAngle aa = rotation.AxisAngle;
            Gl.glRotatef(aa.angle.inDegrees, aa.axis.x, aa.axis.y, aa.axis.z);
        }
    }

    public class EasyRotation : Rotation
    {
        float rx = 0;
        float ry = 0;

        public override void sendMouse(vec2 mouse, vec2 oldmouse)
        {
            float dx =  (mouse.x - oldmouse.x);
            float dy = -(mouse.y - oldmouse.y);
            float sens = 0.1f;
            rx += sens * dx;
            ry += sens * dy;
        }

        public override void rotateGl()
        {
            Gl.glRotatef(-ry, 1, 0, 0);
            Gl.glRotatef(rx, 0, 1, 0);
        }
    }

    public class BasicQuatRot : QuatRot
    {
        public override void sendMouse(vec2 current, vec2 oldmouse)
        {

            vec2 diff = vec2.FromTo(oldmouse, current);
            float dx = diff.x * sens;
            float dy = diff.y * sens;
            quat rx = new quat(AxisAngle.RightHandAround(vec3.Up, angle.FromDegrees(dx)));
            quat ry = new quat(AxisAngle.RightHandAround(vec3.Right, angle.FromDegrees(dy)));
            quat final = rx * ry;
            rotation = quat.Combine(rotation, final);
        }
    }
    public class ArcBallRotation : QuatRot
    {
        Form ba;

        public ArcBallRotation(Form owner)
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
        private vec2 tranform(vec2 v)
        {
            return new vec2(v.x, Height - v.y);
        }

        public override void sendMouse(vec2 current, vec2 oldmouse)
        {
            ArcBall ball = new ArcBall(new vec2(Width / 2.0f, Height / 2.0f), Math.Min(Height, Width) / 2.0f);
            quat final = ball.rotation(tranform(oldmouse), tranform(current));

            rotation = quat.Combine(rotation, final);
        }
    }
}
