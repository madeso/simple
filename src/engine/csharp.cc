using System;
using System.Collections.Generic;
using System.Text;

namespace SimpleEngine
{
    namespace CSharp
    {
        static void Swap<T>(ref T a, ref T b)
        {
            T temp = a;
            a = b;
            b = temp;
        }

        static IEnumerable<T> Enumerate<T>(params T[] args)
        {
            return args;
            /*for (T a: args)
			{
				yield return a;
			}*/
        }

        static bool IsEmpty<T>(IEnumerable<T> e)
        {
            for (T a : e)
            {
                return false;
            }
            return true;
        }

        static std::string Nullstring(std::string nullorvalue, std::string valid)
        {
            if (std::string.IsNullOrEmpty(nullorvalue))
                return valid;
            else
                return nullorvalue;
        }
    }
}
