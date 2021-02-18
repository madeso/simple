using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimpleEngine.fse
{
    public class Map<T>
    {
        Dictionary<string, T> m = new Dictionary<string, T>();
        Func<string, T> generator;

        public Map(Func<string, T> generator)
        {
            this.generator = generator;
        }

        public T get(string var)
        {
            if (m.ContainsKey(var))
            {
                return m[var];
            }

            T t = generator(var);
            add(var, t);
            return t;
        }

        public void add(string name, T t)
        {
            m.Add(name, t);
        }

        public IEnumerable<T> Data
        {
            get
            {
                foreach (KeyValuePair<string, T> kv in m)
                {
                    yield return kv.Value;
                }
            }
        }
    }
}
