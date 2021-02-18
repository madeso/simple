using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimpleEngine
{
    public struct AxisAngle
    {
        private AxisAngle(vec3 axis, angle angle)
        {
            this.axis = axis;
            this.angle = angle;
        }
        public static AxisAngle RightHandAround(vec3 axis, angle angle)
        {
	        return new AxisAngle(axis, angle);
        }

        public override string ToString()
        {
            return axis.ToString() + " " + angle.inDegrees.ToString();
        }

        public AxisAngle Negative
        {
            get
            {
                return new AxisAngle(-axis, -angle);
            }
        }

        public static AxisAngle operator-(AxisAngle me)
        {
            return me.Negative;
        }

        public readonly vec3 axis;
        public readonly angle angle;
    }
}
