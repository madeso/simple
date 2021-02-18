using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimpleEngine
{
    public class Handle<T> : IDisposable where T : Media
    {
        public Handle(T t)
        {
            this.t = t;
            t.increaseUsage();
        }

        private T t;

        public T Ref
        {
            get
            {
                return t;
            }
        }

        public void Dispose()
        {
            t.decreaseUsage();
        }
    }
}
