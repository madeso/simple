using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace SimpleEngine
{
    public static class AnimationFile
    {
        public static Animation Load(FileSystem fs, string path)
        {
            return Binary.Load(fs, path);
        }
        public static void Write(Animation an, Stream s)
        {
            Binary.Write(an, s);
        }

        private class Binary
        {
            public static void Write(Animation an, Stream s)
            {
                BinaryWriter bw = new BinaryWriter(s);
                bw.Write((int)0);
                bw.Write(an.bones.Count);
                foreach (AnimationForBone ab in an.bones)
                {
                    bw.Write(ab.fp.Count);
                    foreach (FramePosition p in ab.fp)
                    {
                        bw.Write(p.time);
                        vec3.Write(p.location, bw);
                    }
                    bw.Write(ab.fr.Count);
                    foreach (FrameRotation r in ab.fr)
                    {
                        bw.Write(r.time);
                        quat.Write(r.rotation, bw);
                    }
                }
            }
            internal static Animation Load(FileSystem fs, string path)
            {
                List<AnimationForBone> afb = new List<AnimationForBone>();
                using (Stream s = fs.open(path))
                {
                    BinaryReader br = new BinaryReader(s);
                    int version = br.ReadInt32();
                    int bonecount = br.ReadInt32();
                    for (int boneid = 0; boneid < bonecount; ++boneid)
                    {
                        AnimationForBone ab = new AnimationForBone();
                        afb.Add(ab);
                        int poscount = br.ReadInt32();
                        for (int posid = 0; posid < poscount; ++posid)
                        {
                            float time = br.ReadSingle();
                            vec3 pos = vec3.Read(br);
                            ab.addPositon(time, pos);
                        }
                        int rotcount = br.ReadInt32();
                        for (int rotid = 0; rotid < rotcount; ++rotid)
                        {
                            float time = br.ReadSingle();
                            quat rot = quat.Read(br);
                            ab.addRotation(time, rot);
                        }
                    }
                }
                return new Animation(afb);
            }
        }
    }
}
