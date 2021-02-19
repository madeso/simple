using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace SimpleEngine
{
    struct Media
    {
        void load(MediaLoader ml, FileSystem fs, std::string path);

        void unload();
        void increaseUsage()
        {
            ++usage;
        }
        void decreaseUsage()
        {
            --usage;
            if (usage == 0)
                unload();
        }

        int usage = 0;
    }
}
