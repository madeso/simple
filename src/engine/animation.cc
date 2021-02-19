#include <stdexcept>
#include <vector>

#include "engine/axisangle.h"
#include "engine/mat44.h"
#include "engine/math1.h"
#include "engine/matrixhelper.h"
#include "engine/meshdef.h"
#include "engine/quat.h"
#include "engine/vec3.h"
#include "fmt/core.h"

namespace SimpleEngine
{
    struct Timed
    {
        float time;

        Timed(float t)
            : time(t)
        {
        }

        template <typename T>
        static int Get(const std::vector<T>& da, float current)
        {
            for (int i = 1; i < da.size(); ++i)
            {
                if (math1::IsWithin(da[i - 1].time, current, da[i].time))
                {
                    return i;
                }
            }

            //throw std::runtime_error("data contatiner invalid for animating");
            return -1;
        }
    };

    struct FramePosition : Timed
    {
        FramePosition(float time, const vec3& l)
            : Timed(time)
            , location(l)
        {
        }
        vec3 location;

        static vec3 Interpolate(const FramePosition& from, float current, const FramePosition& to)
        {
            float scale = math1::To01(from.time, current, to.time);
            if (math1::IsWithin(0, scale, 1) == false)
                throw std::runtime_error("invalid scale");
            return vec3::Interpolate(from.location, scale, to.location);
        }

        std::string ToString() const
        {
            return fmt::format("{0} {1}", time, location.ToString());
        }
    };

    struct FrameRotation : public Timed
    {
        FrameRotation(float time, const quat& r)
            : Timed(time)
            , rotation(r)
        {
        }

        quat rotation;

        static quat Interpolate(FrameRotation from, float current, FrameRotation to)
        {
            float scale = math1::To01(from.time, current, to.time);
            if (math1::IsWithin(0, scale, 1) == false)
                throw std::runtime_error("invalid scale");
            return quat::SlerpShortway(from.rotation, scale, to.rotation);
        }

        std::string ToString() const
        {
            return fmt::format("{0} {1}", time, rotation.AxisAngle().ToString());
        }
    };

    struct PoseForBone
    {
        vec3 location;
        quat rotation;

        std::string ToString() const
        {
            return fmt::format("{0} : {1}", location.ToString(), rotation.AxisAngle().ToString());
        }
    };

    struct AnimationForBone
    {
        std::vector<FramePosition> fp;
        std::vector<FrameRotation> fr;

        std::string ToString() const
        {
            return fmt::format("<{0} {1}>", fp.size(), fr.size());
        }

        float Length() const
        {
            // get the time of the last frame
            return fp[fp.size() - 1].time;
        }

        void addPositon(float time, const vec3& vec3)
        {
            addPositon(FramePosition(time, vec3));
        }
        void addPositon(const FramePosition& ff)
        {
            fp.emplace_back(ff);
        }

        void addRotation(float time, const quat& rotation)
        {
            addRotation(FrameRotation(time, rotation));
        }
        void addRotation(const FrameRotation& ff)
        {
            fr.emplace_back(ff);
        }

        PoseForBone getBonePose(float time) const
        {
            PoseForBone p = PoseForBone();
            p.location = Interpolate(time, fp);
            p.rotation = Interpolate(time, fr);
            return p;
        }

        static quat Interpolate(float time, const std::vector<FrameRotation>& fr)
        {
            int fri = Timed::Get<FrameRotation>(fr, time);
            if (fri == -1)
                return quat::Identity();
            quat r = FrameRotation::Interpolate(fr[fri - 1], time, fr[fri]);
            return r;
        }

        static vec3 Interpolate(float time, const std::vector<FramePosition>& fp)
        {
            int fpi = Timed::Get<FramePosition>(fp, time);
            if (fpi == -1)
                return vec3::Zero();
            vec3 res = FramePosition::Interpolate(fp[fpi - 1], time, fp[fpi]);
            return res;
        }

        AnimationForBone sub(int start, int end)
        {
            AnimationForBone ab;
            float length = end - start;
            bool first = true;
            float last = 0;

            for (auto& fp : this->fp)
            {
                if (math1::IsBetween(start, fp.time, end))
                {
                    float mark = fp.time - start;
                    if (first && math1::IsZero(mark) == false)
                    {
                        ab.addPositon(0, Interpolate(start, this->fp));
                    }
                    mark = math1::ZeroOrValue(mark);
                    first = false;
                    ab.addPositon(mark, fp.location);
                    last = mark;
                }
            }
            if (math1::isSame(length, last) == false)
            {
                ab.addPositon(length, Interpolate(end, this->fp));
            }

            first = true;
            last = 0;

            for (auto& fr : this->fr)
            {
                if (math1::IsBetween(start, fr.time, end))
                {
                    float mark = fr.time - start;
                    if (first && math1::IsZero(mark) == false)
                    {
                        ab.addRotation(0, Interpolate(start, this->fr));
                    }
                    mark = math1::ZeroOrValue(mark);
                    first = false;
                    ab.addRotation(mark, fr.rotation);
                    last = mark;
                }
            }
            if (math1::isSame(length, last) == false)
            {
                ab.addRotation(length, Interpolate(end, this->fr));
            }

            /*if (ab.fp.Count == 1)
            {
                AnimationForBone afb = ab;
                ab = AnimationForBone();
                ab.addPositon(start, afb.fp[0].location);
                ab.addPositon(end, afb.fp[0].location);

                ab.addRotation(start, afb.fr[0].rotation);
                ab.addRotation(end, afb.fr[0].rotation);
            }*/

            if (ab.fp.size() < 2 || ab.fr.size() < 2)
            {
                throw std::runtime_error("Data error, need atleast 2 keyframes per animation");
            }
            return ab;
        }

        void scale(float scale)
        {
            std::vector<FramePosition> nf = std::vector<FramePosition>();
            for (auto& f : fp)
            {
                nf.emplace_back(FramePosition(f.time, f.location * scale));
            }
            fp = nf;
        }
    };

    struct Pose
    {
        Pose(const std::vector<PoseForBone>& pose)
            : bones(pose)
        {
        }

        std::vector<PoseForBone> bones;
    };

    struct CompiledPose
    {
        std::vector<mat44> transforms;

        CompiledPose(const std::vector<mat44>& ts)
            : transforms(ts)
        {
        }

        static CompiledPose Compile(const Pose& pose, const MeshDef& def)
        {
            if (pose.bones.size() != def.bones.size())
            {
                throw std::runtime_error("Invalid animation/mesh, bone count differs");
            }

            auto result = std::vector<mat44>(pose.bones.size());
            for (int i = 0; i < pose.bones.size(); ++i) result.emplace_back(mat44::Identity());
            for (auto& root : def.RootBones())
            {
                updateMatrix(&result, root, pose, def.bones);
            }
            return CompiledPose(result);
        }

        static void updateMatrix(std::vector<mat44>* result, std::shared_ptr<Bone> bone, const Pose& pose, const std::vector<std::shared_ptr<Bone>>& list)
        {
            mat44 parent =
                bone->parentBone == nullptr ? mat44::Identity() : (*result)[bone->parent];
            vec3 loc = pose.bones[bone->index].location;
            quat rot = pose.bones[bone->index].rotation;
            // bone->pos Rotate(-bone->rot).
            (*result)[bone->index] = MatrixHelper(parent).Rotate(bone->rot).Translate(bone->pos).Translate(loc).Rotate(-rot).mat44();
            for (auto& b : bone->childs)
            {
                updateMatrix(result, b, pose, list);
            }
        }
    };

    struct AnimationInformation
    {
        AnimationInformation(int s, int e, const std::string& n)
            : start(s)
            , end(e)
            , name(n)
        {
        }
        int start;
        int end;
        std::string name;
    };

    struct Animation
    {
        std::vector<AnimationForBone> bones;
        float Length;

        Animation(const std::vector<AnimationForBone>& b)
            : bones(b)
            , Length(0.0f)
        {
            for (auto& ab : bones)
            {
                Length = math1::Max(Length, ab.Length());
            }
        }

        Pose getPose(float time) const
        {
            std::vector<PoseForBone> bd;
            for (const auto& ab : bones)
            {
                bd.emplace_back(ab.getBonePose(time));
            }
            return Pose(bd);
        }

        Animation subanim(int start, int end) const
        {
            std::vector<AnimationForBone> bd;
            for (AnimationForBone ab : bones)
            {
                bd.emplace_back(ab.sub(start, end));
            }
            return Animation(bd);
        }

        Animation subanim(const AnimationInformation& info)
        {
            return subanim(info.start, info.end);
        }

        void scale(float scale)
        {
            for (auto& afb : bones)
            {
                afb.scale(scale);
            }
        }
    };
}
