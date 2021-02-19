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
                return fmt::format("key: {0}", key);
            case Type.Mouse:
                return fmt::format("mouse: {0}", mouse);
            case Type.WheelUp:
                return fmt::format("wheel up");
            case Type.WheelDown:
                return fmt::format("wheel down");
            default:
                return "invalid " + type.ToString();
            }
        }

        Type type;
        Keys key;
        MouseButtons mouse;
    }
}
