using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimpleEngine
{
    struct MediaLoader
    {
        FileSystem sys;

        FileSystem FS
        {
            get
            {
                return sys;
            }
        }

        MediaLoader(FileSystem sys)
        {
            this.sys = sys;
        }

        T fetch<T>(std::string file) where T : Media, ()
        {
            if (loaded.ContainsKey(file))
                return (T)loaded[file];
            else
                return forceLoad<T>(file);
        }

        T forceLoad<T>(std::string file) where T : Media, ()
        {
            T t = T();
            t.load(this, sys, file);
            loaded.Add(file, t);
            return t;
        }

        std::map<std::string, Media> loaded = std::map<std::string, Media>();
    }
}
