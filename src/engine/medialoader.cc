using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimpleEngine
{
    public class MediaLoader
    {
        FileSystem sys;

        public FileSystem FS
        {
            get
            {
                return sys;
            }
        }

        public MediaLoader(FileSystem sys)
        {
            this.sys = sys;
        }

        public T fetch<T>(string file) where T : Media, new()
        {
            if (loaded.ContainsKey(file)) return (T)loaded[file];
            else return forceLoad<T>(file);
        }

        private T forceLoad<T>(string file) where T : Media, new()
        {
            T t = new T();
            t.load(this, sys, file);
            loaded.Add(file, t);
            return t;
        }

        private Dictionary<string, Media> loaded = new Dictionary<string, Media>();
    }
}
