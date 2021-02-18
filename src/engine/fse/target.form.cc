using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimpleEngine.fse.Targets
{
    public class FormTarget : Target
    {
        int width;
        int height;

        public FormTarget(int width, int height)
        {
            this.width = width;
            this.height = height;
        }

        public override string ToString()
        {
            return base.ToString() + " targeting a form";
        }

        public override void apply(Action a)
        {
            a();
        }

        public override int Width
        {
            get { return width; }
        }

        public override int Height
        {
            get { return height; }
        }

        public override void link(Linker usr)
        {
        }
    }
}
