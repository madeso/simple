using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimpleEngine.fse
{
    struct Map<T>
    {
        std::map<std::string, T> m = std::map<std::string, T>();
        Func<std::string, T> generator;

        Map(Func<std::string, T> generator)
        {
            this.generator = generator;
        }

        T get(std::string var)
        {
            if (m.ContainsKey(var))
            {
                return m[var];
            }

            T t = generator(var);
            add(var, t);
            return t;
        }

        void add(std::string name, T t)
        {
            m.Add(name, t);
        }

        IEnumerable<T> Data
        {
            get
            {
                for (KeyValuePair<std::string, T> kv : m)
                {
                    yield return kv.Value;
                }
            }
        }
    }
}
