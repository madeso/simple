using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace SimpleEngine
{
    public struct Ckey
    {
        public bool equals(Ckey b)
        {
            return b.type == type
                && b.key == key && b.mouse == mouse;
        }

        public Ckey(Keys key)
        {
            this.type = Type.Key;
            this.key = key;
            this.mouse = MouseButtons.None;
        }
        public Ckey(MouseButtons mouse)
        {
            this.type = Type.Mouse;
            this.key = Keys.None;
            this.mouse = mouse;
        }

        public static Ckey WheelUp
        {
            get
            {
                return new Ckey(true);
            }
        }
        public static Ckey WheelDown
        {
            get
            {
                return new Ckey(false);
            }
        }

        private Ckey(bool up)
        {
            this.type = (up) ? Type.WheelUp : Type.WheelDown;
            this.key = Keys.None;
            this.mouse = MouseButtons.None;
        }

        public enum Type
        {
            Key
          , Mouse
          , WheelUp
          , WheelDown
        }

        public override string ToString()
        {
            switch (type)
            {
                case Type.Key:
                    return string.Format("key: {0}", key);
                case Type.Mouse:
                    return string.Format("mouse: {0}", mouse);
                case Type.WheelUp:
                    return string.Format("wheel up");
                case Type.WheelDown:
                    return string.Format("wheel down");
                default:
                    return "invalid " + type.ToString();
            }
        }

        public readonly Type type;
        public readonly Keys key;
        public readonly MouseButtons mouse;
    }
}
