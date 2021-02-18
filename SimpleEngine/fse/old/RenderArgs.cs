using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimpleEngine.fse.old
{
    internal class RenderArgs
    {
        Component last;
        RenderTarget target;
        User usr;

        public RenderArgs(User usr, RenderTarget rt, Component last)
        {
            this.usr = usr;
            this.target = rt;
            this.last = last;
        }

        internal Fbo getBuffer(string buffer)
        {
            return usr.getBuffer(buffer);
        }

        internal void apply(Shader shader, string target, Component last)
        {
            if (last == this.last) this.target.render(shader);
            usr.apply(shader, target);
        }
    }
}
