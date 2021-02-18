using System;
using System.Collections.Generic;
using System.Text;

namespace SimpleEngine
{
	public class StringSeperator
	{
		public StringSeperator(string aSeperator, string aFinalSeperator, string empty)
		{
			mSeperator = aSeperator;
			mFinalSeperator = aFinalSeperator;
			mEmpty = empty;
		}
		public StringSeperator(string aSeperator, string aFinalSeperator)
		{
			mSeperator = aSeperator;
			mFinalSeperator = aFinalSeperator;
			mEmpty = "";
		}
		public StringSeperator(string aSeperator)
		{
			mSeperator = aSeperator;
			mFinalSeperator = aSeperator;
		}

		public StringSeperator Append(string astring)
		{
			mstrings.Add(astring);
			return this;
		}

		public StringSeperator Append(IEnumerable<string> astrings)
		{
			mstrings.AddRange(astrings);
			return this;
		}
		public StringSeperator Append(object[] d)
		{
			foreach (object o in d)
				mstrings.Add(o.ToString());
			return this;
		}

		public StringSeperator AppendFormat(string aFormat, params Object[] aValues)
		{
			StringBuilder builder = new StringBuilder();
			builder.AppendFormat(aFormat, aValues);
			Append(builder.ToString());
			return this;
		}

		public void Clear()
		{
			mstrings.Clear();
		}

		public override string ToString()
		{
			if (mstrings.Count == 0) return mEmpty;
			StringBuilder builder = new StringBuilder();
			for (int index = 0; index < mstrings.Count; ++index)
			{
				string value = mstrings[index];
				builder.Append(value);

				if (mstrings.Count != index + 1) // if this item isnt the last one in the list
				{
					string seperator = mSeperator;
					if (mstrings.Count == index + 2)
					{
						seperator = mFinalSeperator;
					}
					builder.Append(seperator);
				}
			}
			return builder.ToString();
		}

		List<string> mstrings = new List<string>();
		readonly string mSeperator;
		readonly string mFinalSeperator;
		readonly string mEmpty;
	}
}
