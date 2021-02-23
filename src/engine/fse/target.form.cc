using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimpleEngine::fse.Targets
{
    struct FormTarget : Target
    {
        int width;
        int height;

        FormTarget(int width, int height)
        {
            this.width = width;
            this.height = height;
        }

        std::string ToString() const
        {
            return base.ToString() + " targeting a form";
        }

        override void apply(Action a)
        {
            a();
        }

        override int Width
        {
            get { return width; }
        }

        override int Height
        {
            get { return height; }
        }

        override void link(Linker usr)
        {
        }
    }
}
