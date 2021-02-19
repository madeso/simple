using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimpleEngine
{
    struct StringCounter
    {
        std::map<std::string, int> counts = std::map<std::string, int>();

        void add(std::string s)
        {
            int c = countsOf(s);

            if (c > 0)
            {
                counts.Remove(s);
            }

            int nc = c + 1;
            counts.Add(s, nc);
        }

        int countsOf(std::string s)
        {
            if (counts.ContainsKey(s))
            {
                return counts[s];
            }
            else
            {
                return 0;
            }
        }
    }
}
