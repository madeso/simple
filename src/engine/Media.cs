using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace SimpleEngine
{
    public abstract class Media
    {
        public abstract void load(MediaLoader ml, FileSystem fs, string path);
        protected abstract void unload();
        public void increaseUsage()
        {
            ++usage;
        }
        public void decreaseUsage()
        {
            --usage;
            if (usage == 0) unload();
        }
        private int usage = 0;
    }
}
