using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimpleEngine
{
    struct Handle<T> : IDisposable where T : Media
    {
        Handle(T t)
        {
            this.t = t;
            t.increaseUsage();
        }

        T t;

        T Ref
        {
            get
            {
                return t;
            }
        }

        void Dispose()
        {
            t.decreaseUsage();
        }
    }
}
