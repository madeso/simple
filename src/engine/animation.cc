#include "engine/animation.h"

#include <stdexcept>
#include <vector>

#include "engine/axisangle.h"
#include "engine/math.h"
#include "engine/matrixhelper.h"
#include "engine/meshdef.h"
#include "fmt/core.h"

namespace simple
{
    namespace timed
    {
        template <typename T>
        int Get(const std::vector<T>& da, float current)
        {
            for (int i = 1; i < da.size(); ++i)
            {
                if (math::IsWithin(da[i - 1].time, current, da[i].time))
                {
                    return i;
                }
            }

            // throw std::runtime_error("data contatiner invalid for animating");
            return -1;
        }
    }

    FramePosition::FramePosition(float t, const vec3& l)
        : time(t)
        , location(l)
    {
    }

    vec3 FramePosition::Interpolate(const FramePosition& from, float current, const FramePosition& to)
    {
        float scale = math::To01(from.time, current, to.time);
        if (math::IsWithin(0, scale, 1) == false)
            throw std::runtime_error("invalid scale");
        return vec3::Interpolate(from.location, scale, to.location);
    }

    std::string FramePosition::ToString() const
    {
        return fmt::format("{0} {1}", time, location.ToString());
    }

    FrameRotation::FrameRotation(float t, const quat& r)
        : time(t)
        , rotation(r)
    {
    }

    quat FrameRotation::Interpolate(FrameRotation from, float current, FrameRotation to)
    {
        float scale = math::To01(from.time, current, to.time);
        if (math::IsWithin(0, scale, 1) == false)
            throw std::runtime_error("invalid scale");
        return quat::SlerpShortway(from.rotation, scale, to.rotation);
    }

    std::string FrameRotation::ToString() const
    {
        return fmt::format("{0} {1}", time, rotation.GetAxisAngle().ToString());
    }

    std::string PoseForBone::ToString() const
    {
        return fmt::format("{0} : {1}", location.ToString(), rotation.GetAxisAngle().ToString());
    }

    std::string AnimationForBone::ToString() const
    {
        return fmt::format("<{0} {1}>", fp.size(), fr.size());
    }

    float AnimationForBone::GetLength() const
    {
        // get the time of the last frame
        return fp[fp.size() - 1].time;
    }

    void AnimationForBone::AddPositon(float time, const vec3& vec3)
    {
        AddPositon(FramePosition(time, vec3));
    }
    void AnimationForBone::AddPositon(const FramePosition& ff)
    {
        fp.emplace_back(ff);
    }

    void AnimationForBone::AddRotation(float time, const quat& rotation)
    {
        AddRotation(FrameRotation(time, rotation));
    }
    void AnimationForBone::AddRotation(const FrameRotation& ff)
    {
        fr.emplace_back(ff);
    }

    PoseForBone AnimationForBone::GetBonePose(float time) const
    {
        PoseForBone p = PoseForBone();
        p.location = Interpolate(time, fp);
        p.rotation = Interpolate(time, fr);
        return p;
    }

    quat AnimationForBone::Interpolate(float time, const std::vector<FrameRotation>& fr)
    {
        int fri = timed::Get<FrameRotation>(fr, time);
        if (fri == -1)
            return quat::Identity();
        quat r = FrameRotation::Interpolate(fr[fri - 1], time, fr[fri]);
        return r;
    }

    vec3 AnimationForBone::Interpolate(float time, const std::vector<FramePosition>& fp)
    {
        int fpi = timed::Get<FramePosition>(fp, time);
        if (fpi == -1)
            return vec3::Zero();
        vec3 res = FramePosition::Interpolate(fp[fpi - 1], time, fp[fpi]);
        return res;
    }

    AnimationForBone AnimationForBone::GetSubAnimation(int start, int end)
    {
        AnimationForBone ab;
        float length = end - start;
        bool first = true;
        float last = 0;

        for (auto& fp : this->fp)
        {
            if (math::IsBetween(start, fp.time, end))
            {
                float mark = fp.time - start;
                if (first && math::IsZero(mark) == false)
                {
                    ab.AddPositon(0, Interpolate(start, this->fp));
                }
                mark = math::ZeroOrValue(mark);
                first = false;
                ab.AddPositon(mark, fp.location);
                last = mark;
            }
        }
        if (math::IsSame(length, last) == false)
        {
            ab.AddPositon(length, Interpolate(end, this->fp));
        }

        first = true;
        last = 0;

        for (auto& fr : this->fr)
        {
            if (math::IsBetween(start, fr.time, end))
            {
                float mark = fr.time - start;
                if (first && math::IsZero(mark) == false)
                {
                    ab.AddRotation(0, Interpolate(start, this->fr));
                }
                mark = math::ZeroOrValue(mark);
                first = false;
                ab.AddRotation(mark, fr.rotation);
                last = mark;
            }
        }
        if (math::IsSame(length, last) == false)
        {
            ab.AddRotation(length, Interpolate(end, this->fr));
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

    void AnimationForBone::Scale(float scale)
    {
        std::vector<FramePosition> nf = std::vector<FramePosition>();
        for (auto& f : fp)
        {
            nf.emplace_back(FramePosition(f.time, f.location * scale));
        }
        fp = nf;
    }

    Pose::Pose(const std::vector<PoseForBone>& pose)
        : bones(pose)
    {
    }

    CompiledPose::CompiledPose(const std::vector<mat44>& ts)
        : transforms(ts)
    {
    }

    void UpdateMatrix(std::vector<mat44>* result, std::shared_ptr<Bone> bone, const Pose& pose)
    {
        mat44 parent =
            bone->parent_bone == nullptr ? mat44::Identity() : (*result)[bone->parent];
        vec3 loc = pose.bones[bone->index].location;
        quat rot = pose.bones[bone->index].rotation;
        (*result)[bone->index] = MatrixHelper(parent).Rotate(bone->rotation).Translate(bone->position).Translate(loc).Rotate(-rot).AsMat44();
        for (auto& b : bone->children)
        {
            UpdateMatrix(result, b, pose);
        }
    }

    CompiledPose CompiledPose::Compile(const Pose& pose, const MeshDef& def)
    {
        if (pose.bones.size() != def.bones.size())
        {
            throw std::runtime_error("Invalid animation/mesh, bone count differs");
        }

        auto result = std::vector<mat44>(pose.bones.size());
        for (int i = 0; i < pose.bones.size(); ++i) result.emplace_back(mat44::Identity());
        for (auto& root : def.GetRootBones())
        {
            UpdateMatrix(&result, root, pose);
        }
        return CompiledPose(result);
    }

    AnimationInformation::AnimationInformation(int s, int e, const std::string& n)
        : start(s)
        , end(e)
        , name(n)
    {
    }

    Animation::Animation(const std::vector<AnimationForBone>& b)
        : bones(b)
        , length(0.0f)
    {
        for (auto& ab : bones)
        {
            length = math::Max(length, ab.GetLength());
        }
    }

    Pose Animation::GetPose(float time) const
    {
        std::vector<PoseForBone> bd;
        for (const auto& ab : bones)
        {
            bd.emplace_back(ab.GetBonePose(time));
        }
        return Pose(bd);
    }

    Animation Animation::GetSubAnimation(int start, int end) const
    {
        std::vector<AnimationForBone> bd;
        for (AnimationForBone ab : bones)
        {
            bd.emplace_back(ab.GetSubAnimation(start, end));
        }
        return Animation(bd);
    }

    Animation Animation::GetSubAnimation(const AnimationInformation& info)
    {
        return GetSubAnimation(info.start, info.end);
    }

    void Animation::Scale(float scale)
    {
        for (auto& afb : bones)
        {
            afb.Scale(scale);
        }
    }
}
