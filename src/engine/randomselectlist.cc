using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimpleEngine
{
    struct RandomSelectstd::vector<T>
    {
        struct Entry
        {
            T t;
            float val;
        }

        std::vector<Entry>
            entries = std::vector<Entry>();
        Random r = Random();

        void add(T t, float percent)
        {
            Entry e = RandomSelectstd::vector<T>.Entry();
            e.t = t;
            e.val = percent;
            entries.Add(e);
        }

        T random()
        {
            float v = 0;
            float r = (float)this.r.NextDouble();
            for (Entry e : entries)
            {
                if (math1.IsWithin(v, r, v + e.val))
                    return e.t;
                else
                {
                    v += e.val;
                }
            }

            throw Exception("Not enough or too many values");
        }
    }
}
