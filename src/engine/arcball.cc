using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimpleEngine
{
    struct ArcBall
    {
        ArcBall(vec2 center, float radius)
        {
            this.radius = radius;
            this.center = center;
        }

        vec3 transform(vec2 v)
        {
            float x = (v.x - center.x) / radius;
            float y = (v.y - center.y) / radius;
            float rsq = x * x + y * y;
            float z = (rsq < 1) ? (float)(Math.Sqrt(1 - rsq)) : 0;
            return vec3(x, y, z).Normalized;
        }

        quat rotation(vec2 from, vec2 to)
        {
            vec3 f = transform(from);
            vec3 t = transform(to);
            vec3 axis = vec3.cross(f, t).Normalized;
            angle angle = vec3.AngleBetween(f, t);
            return quat(AxisAngle.RightHandAround(axis, angle));
        }

        vec2 center;

        float radius;
    }
}
