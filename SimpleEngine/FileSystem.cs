using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace SimpleEngine
{
    public class FileSystem
    {
        public FileSystem()
        {
        }

        public void addRoot(string root)
        {
            roots.Add(root);
        }

        public void addDefaultRoots(string app, string game)
        {
            addRoot(Environment.CurrentDirectory);
            addRoot(combine(Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData), app, game));
            addRoot(combine(Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData), app, game));
            addRoot(combine(Environment.GetFolderPath(Environment.SpecialFolder.CommonApplicationData), app, game));
            addRoot(combine(Environment.GetFolderPath(Environment.SpecialFolder.ProgramFiles), app, game));
            addRoot(combine(Environment.GetFolderPath(Environment.SpecialFolder.Programs), app, game));
        }

        private Stream doOpen(string path)
        {
            if (path.Contains("..")) throw new Exception("do not specify parent directory");
            string p = path;//.Replace(':', Path.DirectorySeparatorChar);
            foreach (string r in roots)
            {
                string fp = Path.Combine(r, p);
                if (File.Exists(fp)) return new FileStream(fp, FileMode.Open);
            }
            throw new Exception("Failed to open file " + path);
        }

        public Stream open(string path)
        {
            return doOpen(MapFile(overrides, path));
        }

        private string combine(string s, params string[] args)
        {
            string r = s;
            foreach (string a in args)
            {
                r = Path.Combine(r, a);
            }
            return r;
        }

        List<string> roots = new List<string>();

        public IEnumerable<string> readLines(string path)
        {
            using (Stream s = open(path))
            {
                using (StreamReader sr = new StreamReader(s))
                {
                    while (sr.EndOfStream == false)
                    {
                        yield return sr.ReadLine();
                    }
                }
            }
        }

        Dictionary<string, string> overrides = null;

        internal void setOverrides(Dictionary<string, string> overrides)
        {
            this.overrides = overrides;
        }

        internal void clearOverrides(Dictionary<string, string> overrides)
        {
            this.overrides = null;
        }

        internal static string MapFile(Dictionary<string, string> overrides, string path)
        {
            string finalpath = path.ToLower();
            if (overrides != null)
            {
                while (overrides.ContainsKey(finalpath))
                {
                    finalpath = overrides[finalpath];
                }
            }
            return finalpath;
        }
    }
}
