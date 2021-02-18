using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using System.IO;

namespace SimpleEngine.load
{
    class Binary
    {
        Stream fs;

        public Binary(Stream s)
        {
            this.fs = s;
        }

        public Binary(byte[] bytes)
        {
            fs = new MemoryStream(bytes, false);
        }
        /*

        public T Read<T>()
        {
            byte[] b = Read(Marshal.SizeOf(typeof(T)));
            return (T)RawDeserializeEx(b, typeof(T));
        }*/
        public T Read<T>(int size)
        {
            byte[] b = Read(size);
            return (T)RawDeserializeEx(b, typeof(T));
        }

        public bool Continue
        {
            get
            {
                return fs.Position < fs.Length;
            }
        }

        public byte[] Read(long size)
        {
            byte[] b = new byte[size];
            int read = fs.Read(b, 0, b.Length);
            if (size != read) throw new Exception("Not enough bytes read");
            return b;
        }

        private object RawDeserializeEx(byte[] rawdatas, Type anytype)
        {
            int rawsize = Marshal.SizeOf(anytype);
            if (rawsize > rawdatas.Length)
                return null;
            GCHandle handle = GCHandle.Alloc(rawdatas, GCHandleType.Pinned);
            IntPtr buffer = handle.AddrOfPinnedObject();
            object retobj = Marshal.PtrToStructure(buffer, anytype);
            handle.Free();
            return retobj;
        }

        private static string CropNull(string input)
        {
            input = input.Trim();
            if (input.EndsWith(((char)13).ToString())
                && input.IndexOf('\0') == -1)
            {
                return input.Substring(0, input.Length - 1);
            }
            if (input.IndexOf('\0') == -1)
                return input;
            return input.Substring(0, input.IndexOf('\0'));
        }

        /*public string ReadString(int count)
        {
            byte[] b = new byte[count];
            fs.Read(b, 0, b.Length);
            return  CropNull(System.Text.Encoding.UTF8.GetString(b));
        }*/
        public string ReadString()
        {
            List<byte> bytes = new List<byte>();
            byte[] b = new byte[1];
            
            do
            {
                fs.Read(b, 0, 1);
                bytes.Add(b[0]);
            } while (b[0] != 0);

            return CropNull(System.Text.Encoding.UTF8.GetString(bytes.ToArray()));
        }

        public int int2()
        {
            return Read<ushort>(2);
        }
        public int int4()
        {
            return Read<int>(4);
        }

        internal float rfloat()
        {
            return Read<float>(4);
        }
    }
}
