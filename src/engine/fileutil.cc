using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace SimpleEngine
{
    namespace FileUtil
    {
        static IEnumerable<std::string> LinesIn(Stream s)
        {
            StreamReader sr = StreamReader(s);
            while (false == sr.EndOfStream)
            {
                yield return sr.ReadLine();
            }
        }
    }
}
