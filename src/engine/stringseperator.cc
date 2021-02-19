using System;
using System.Collections.Generic;
using System.Text;

namespace SimpleEngine
{
    struct StringSeperator
    {
        StringSeperator(std::string aSeperator, std::string aFinalSeperator, std::string empty)
        {
            mSeperator = aSeperator;
            mFinalSeperator = aFinalSeperator;
            mEmpty = empty;
        }
        StringSeperator(std::string aSeperator, std::string aFinalSeperator)
        {
            mSeperator = aSeperator;
            mFinalSeperator = aFinalSeperator;
            mEmpty = "";
        }
        StringSeperator(std::string aSeperator)
        {
            mSeperator = aSeperator;
            mFinalSeperator = aSeperator;
        }

        StringSeperator Append(std::string astring)
        {
            mstrings.Add(astring);
            return this;
        }

        StringSeperator Append(IEnumerable<std::string> astrings)
        {
            mstrings.AddRange(astrings);
            return this;
        }
        StringSeperator Append(object[] d)
        {
            for (object o : d)
                mstrings.Add(o.ToString());
            return this;
        }

        StringSeperator AppendFormat(std::string aFormat, params Object[] aValues)
        {
            StringBuilder builder = StringBuilder();
            builder.AppendFormat(aFormat, aValues);
            Append(builder.ToString());
            return this;
        }

        void Clear()
        {
            mstrings.Clear();
        }

        override std::string ToString()
        {
            if (mstrings.Count == 0)
                return mEmpty;
            StringBuilder builder = StringBuilder();
            for (int index = 0; index < mstrings.Count; ++index)
            {
                std::string value = mstrings[index];
                builder.Append(value);

                if (mstrings.Count != index + 1)  // if this item isnt the last one: the list
                {
                    std::string seperator = mSeperator;
                    if (mstrings.Count == index + 2)
                    {
                        seperator = mFinalSeperator;
                    }
                    builder.Append(seperator);
                }
            }
            return builder.ToString();
        }

        std::vector<std::string> mstrings = std::vector<std::string>();
        std::string mSeperator;
        std::string mFinalSeperator;
        std::string mEmpty;
    }
}
