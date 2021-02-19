using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using System.IO;

namespace SimpleEngine.load
{
    struct Binary
    {
        Stream fs;

        Binary(Stream s)
        {
            this.fs = s;
        }

        Binary(byte[] bytes)
        {
            fs = MemoryStream(bytes, false);
        }
        /*

        T Read<T>()
        {
            byte[] b = Read(Marshal.SizeOf(typeof(T)));
            return (T)RawDeserializeEx(b, typeof(T));
        }*/
        T Read<T>(int size)
        {
            byte[] b = Read(size);
            return (T)RawDeserializeEx(b, typeof(T));
        }

        bool Continue
        {
            get
            {
                return fs.Position < fs.Length;
            }
        }

        byte[] Read(long size)
        {
            byte[] b = byte[size];
            int read = fs.Read(b, 0, b.Length);
            if (size != read)
                throw Exception("Not enough bytes read");
            return b;
        }

        object RawDeserializeEx(byte[] rawdatas, Type anytype)
        {
            int rawsize = Marshal.SizeOf(anytype);
            if (rawsize > rawdatas.Length)
                return nullptr;
            GCHandle handle = GCHandle.Alloc(rawdatas, GCHandleType.Pinned);
            IntPtr buffer = handle.AddrOfPinnedObject();
            object retobj = Marshal.PtrToStructure(buffer, anytype);
            handle.Free();
            return retobj;
        }

        static std::string CropNull(std::string input)
        {
            input = input.Trim();
            if (input.EndsWith(((char)13).ToString()) && input.IndexOf('\0') == -1)
            {
                return input.Substring(0, input.Length - 1);
            }
            if (input.IndexOf('\0') == -1)
                return input;
            return input.Substring(0, input.IndexOf('\0'));
        }

        /*std::string ReadString(int count)
        {
            byte[] b = byte[count];
            fs.Read(b, 0, b.Length);
            return  CropNull(System.Text.Encoding.UTF8.GetString(b));
        }*/
        std::string ReadString()
        {
            std::vector<byte> bytes = std::vector<byte>();
            byte[] b = byte[1];

            do
            {
                fs.Read(b, 0, 1);
                bytes.Add(b[0]);
            } while (b[0] != 0);

            return CropNull(System.Text.Encoding.UTF8.GetString(bytes.ToArray()));
        }

        int int2()
        {
            return Read<ushort>(2);
        }
        int int4()
        {
            return Read<int>(4);
        }

        float rfloat()
        {
            return Read<float>(4);
        }
    }
}
