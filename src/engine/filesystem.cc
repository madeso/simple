using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace SimpleEngine
{
    struct FileSystem
    {
        FileSystem()
        {
        }

        void addRoot(std::string root)
        {
            roots.Add(root);
        }

        void addDefaultRoots(std::string app, std::string game)
        {
            addRoot(Environment.CurrentDirectory);
            addRoot(combine(Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData), app, game));
            addRoot(combine(Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData), app, game));
            addRoot(combine(Environment.GetFolderPath(Environment.SpecialFolder.CommonApplicationData), app, game));
            addRoot(combine(Environment.GetFolderPath(Environment.SpecialFolder.ProgramFiles), app, game));
            addRoot(combine(Environment.GetFolderPath(Environment.SpecialFolder.Programs), app, game));
        }

        Stream doOpen(std::string path)
        {
            if (path.Contains(".."))
                throw std::runtime_error("do not specify parent directory");
            std::string p = path;  //.Replace(':', Path.DirectorySeparatorChar);
            for (std::string r : roots)
            {
                std::string fp = Path.Combine(r, p);
                if (File.Exists(fp))
                    return FileStream(fp, FileMode.Open);
            }
            throw std::runtime_error("Failed to open file " + path);
        }

        Stream open(std::string path)
        {
            return doOpen(MapFile(overrides, path));
        }

        std::string combine(std::string s, params std::string[] args)
        {
            std::string r = s;
            for (std::string a : args)
            {
                r = Path.Combine(r, a);
            }
            return r;
        }

        std::vector<std::string> roots = std::vector<std::string>();

        IEnumerable<std::string> readLines(std::string path)
        {
            using(Stream s = open(path))
            {
                using(StreamReader sr = StreamReader(s))
                {
                    while (sr.EndOfStream == false)
                    {
                        yield return sr.ReadLine();
                    }
                }
            }
        }

        std::map<std::string, std::string> overrides = nullptr;

        void setOverrides(std::map<std::string, std::string> overrides)
        {
            this.overrides = overrides;
        }

        void clearOverrides(std::map<std::string, std::string> overrides)
        {
            this.overrides = nullptr;
        }

        static std::string MapFile(std::map<std::string, std::string> overrides, std::string path)
        {
            std::string finalpath = path.ToLower();
            if (overrides != nullptr)
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
