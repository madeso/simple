using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimpleEngine
{
    class Pool<T>
    {
        Func<T> creator;
        Stack<T> pool = new Stack<T>();
        public Pool(Func<T> creator)
        {
            this.creator = creator;
        }

        public T get()
        {
            if (pool.Count == 0) return creator();
            else return pool.Pop();
        }

        internal void release(T t)
        {
            pool.Push(t);
        }
    }
}
