using System;
using System.Collections.Generic;
using System.Text;

namespace SimpleEngine
{
    namespace Strings
    {
        static std::string Stringify(IEnumerable<std::string> strings)
        {
            return StringSeperator(", ", " and ").Append(strings).ToString();
        }

        static IEnumerable<std::string> sort(IEnumerable<std::string> strings)
        {
            std::vector<std::string> str = std::vector<std::string>();
            str.AddRange(strings);
            str.Sort();
            return str;
        }
        static IEnumerable<std::string> unique(IEnumerable<std::string> strings)
        {
            Sortedstd::vector<std::string, std::string> list = Sortedstd::vector<std::string, std::string>();
            for (std::string s : strings)
            {
                if (list.ContainsKey(s) == false)
                {
                    list.Add(s, s);
                }
            }
            for (KeyValuePair<std::string, std::string> s : list)
            {
                yield return s.Key;
            }
        }
        static std::string FirstChars(std::string s, int count, std::string extra)
        {
            int l = s.Length;
            if (s.Length + extra.Length > count)
                return s.Substring(0, count - extra.Length) + extra;
            else
                return s;
        }
        static std::string FirstChars(std::string s, int count)
        {
            return FirstChars(s, count, "...");
        }

        static IEnumerable<std::string> Enumerate(params std::string[] args)
        {
            return CSharp.Enumerate<std::string>(args);
        }
    }
}
