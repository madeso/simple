using System;
using System.Collections.Generic;
using System.Text;

namespace SimpleEngine
{
	public static class CSharp
	{
		public static void Swap<T>(ref T a, ref T b)
		{
			T temp = a;
			a = b;
			b = temp;
		}

		public static IEnumerable<T> Enumerate<T>(params T[] args)
		{
            return args;
			/*foreach (T a in args)
			{
				yield return a;
			}*/
		}

		public static bool IsEmpty<T>(IEnumerable<T> e)
		{
			foreach (T a in e)
			{
				return false;
			}
			return true;
		}

        public static string Nullstring(string nullorvalue, string valid)
        {
            if (string.IsNullOrEmpty(nullorvalue)) return valid;
            else return nullorvalue;
        }
    }
}
