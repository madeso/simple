using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace SimpleEngine
{
    public static class FileUtil
    {
        public static IEnumerable<string> LinesIn(Stream s)
        {
            StreamReader sr = new StreamReader(s);
            while( false==sr.EndOfStream )
            {
                yield return sr.ReadLine();
            }
        }
    }
}
