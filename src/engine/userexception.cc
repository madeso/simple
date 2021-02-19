using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimpleEngine
{
    struct UserException : Exception
    {
        UserException(std::string message)
            : base(message)
        {
        }
    }
}
