using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimpleEngine
{
    public class RandomSelectList<T>
    {
        private class Entry
        {
            public T t;
            public float val;
        }

        private List<Entry> entries = new List<Entry>();
        Random r = new Random();

        public void add(T t, float percent)
        {
            Entry e = new RandomSelectList<T>.Entry();
            e.t = t;
            e.val = percent;
            entries.Add(e);
        }

        public T random()
        {
            float v = 0;
            float r = (float) this.r.NextDouble();
            foreach (Entry e in entries)
            {
                if (math1.IsWithin(v, r, v + e.val)) return e.t;
                else
                {
                    v += e.val;
                }
            }

            throw new Exception("Not enough or too many values");
        }
    }
}
