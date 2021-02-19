using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace SimpleEngine
{
    struct Ckey
    {
        bool equals(Ckey b)
        {
            return b.type == type && b.key == key && b.mouse == mouse;
        }

        Ckey(Keys key)
        {
            this.type = Type.Key;
            this.key = key;
            this.mouse = MouseButtons.None;
        }
        Ckey(MouseButtons mouse)
        {
            this.type = Type.Mouse;
            this.key = Keys.None;
            this.mouse = mouse;
        }

        static Ckey WheelUp
        {
            get
            {
                return Ckey(true);
            }
        }
        static Ckey WheelDown
        {
            get
            {
                return Ckey(false);
            }
        }

        Ckey(bool up)
        {
            this.type = (up) ? Type.WheelUp : Type.WheelDown;
            this.key = Keys.None;
            this.mouse = MouseButtons.None;
        }

        enum Type
        {
            Key,
            Mouse,
            WheelUp,
            WheelDown
        }

        override std::string
        ToString()
        {
            switch (type)
            {
            case Type.Key:
                return std::string.Format("key: {0}", key);
            case Type.Mouse:
                return std::string.Format("mouse: {0}", mouse);
            case Type.WheelUp:
                return std::string.Format("wheel up");
            case Type.WheelDown:
                return std::string.Format("wheel down");
            default:
                return "invalid " + type.ToString();
            }
        }

        Type type;
        Keys key;
        MouseButtons mouse;
    }
}
