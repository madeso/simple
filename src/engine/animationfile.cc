#include "engine/animationfile.h"

#include "engine/animation.h"
#include "engine/filesystem.h"
#include "engine/fileutil.h"

namespace SimpleEngine
{
    namespace AnimationFile
    {
        namespace Binary
        {
            void Write(std::shared_ptr<Animation> an, const std::string& s)
            {
                BinaryWriter bw = BinaryWriter(s);
                bw.WriteInt32(0);
                bw.WriteInt32(an->bones.size());
                for (const auto& ab : an->bones)
                {
                    bw.WriteInt32(ab.fp.size());
                    for (const auto& p : ab.fp)
                    {
                        bw.WriteSingle(p.time);
                        vec3::Write(p.location, bw);
                    }
                    bw.WriteInt32(ab.fr.size());
                    for (const auto& r : ab.fr)
                    {
                        bw.WriteSingle(r.time);
                        quat::Write(r.rotation, bw);
                    }
                }
            }

            std::shared_ptr<Animation> Load(FileSystem* fs, const std::string& path)
            {
                std::vector<AnimationForBone> afb;
                auto s = fs->open(path);
                {
                    BinaryReader br = BinaryReader(s);
                    int version = br.ReadInt32();
                    int bonecount = br.ReadInt32();
                    for (int boneid = 0; boneid < bonecount; ++boneid)
                    {
                        afb.emplace_back();
                        AnimationForBone& ab = *afb.rbegin();

                        int poscount = br.ReadInt32();
                        for (int posid = 0; posid < poscount; ++posid)
                        {
                            float time = br.ReadSingle();
                            vec3 pos = vec3::Read(br);
                            ab.addPositon(time, pos);
                        }
                        int rotcount = br.ReadInt32();
                        for (int rotid = 0; rotid < rotcount; ++rotid)
                        {
                            float time = br.ReadSingle();
                            quat rot = quat::Read(br);
                            ab.addRotation(time, rot);
                        }
                    }
                }
                return std::make_shared<Animation>(afb);
            }
        }

        std::shared_ptr<Animation> Load(FileSystem* fs, const std::string& path)
        {
            return Binary::Load(fs, path);
        }

        void Write(std::shared_ptr<Animation> an, const std::string& s)
        {
            Binary::Write(an, s);
        }
    }
}
