using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimpleEngine
{
    public class StringCounter
    {
        Dictionary<string, int> counts = new Dictionary<string, int>();

        public void add(string s)
        {
            int c = countsOf(s);
            
            if (c > 0)
            {
                counts.Remove(s);
            }

            int nc = c + 1;
            counts.Add(s, nc);
        }

        public int countsOf(string s)
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
