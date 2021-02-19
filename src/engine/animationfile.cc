using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace SimpleEngine
{
    namespace AnimationFile
    {
        static Animation Load(FileSystem fs, std::string path)
        {
            return Binary.Load(fs, path);
        }
        static void Write(Animation an, Stream s)
        {
            Binary.Write(an, s);
        }

        struct Binary
        {
            static void Write(Animation an, Stream s)
            {
                BinaryWriter bw = BinaryWriter(s);
                bw.Write((int)0);
                bw.Write(an.bones.Count);
                for (AnimationForBone ab : an.bones)
                {
                    bw.Write(ab.fp.Count);
                    for (FramePosition p : ab.fp)
                    {
                        bw.Write(p.time);
                        vec3.Write(p.location, bw);
                    }
                    bw.Write(ab.fr.Count);
                    for (FrameRotation r : ab.fr)
                    {
                        bw.Write(r.time);
                        quat.Write(r.rotation, bw);
                    }
                }
            }
            static Animation Load(FileSystem fs, std::string path)
            {
                std::vector<AnimationForBone> afb = std::vector<AnimationForBone>();
                using(Stream s = fs.open(path))
                {
                    BinaryReader br = BinaryReader(s);
                    int version = br.ReadInt32();
                    int bonecount = br.ReadInt32();
                    for (int boneid = 0; boneid < bonecount; ++boneid)
                    {
                        AnimationForBone ab = AnimationForBone();
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
                return Animation(afb);
            }
        }
    }
}
