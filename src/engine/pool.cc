using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimpleEngine
{
    struct Pool<T>
    {
        Func<T> creator;
        Stack<T> pool = Stack<T>();
        Pool(Func<T> creator)
        {
            this.creator = creator;
        }

        T get()
        {
            if (pool.Count == 0)
                return creator();
            else
                return pool.Pop();
        }

        void release(T t)
        {
            pool.Push(t);
        }
    }
}
