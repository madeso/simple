#include <memory>
#include <vector>

#include "engine/mat44.h"
#include "engine/quat.h"
#include "engine/vec3.h"

namespace SimpleEngine
{
    struct MeshDef;
    struct Bone;

    struct FramePosition
    {
        FramePosition(float time, const vec3& l);

        float time;
        vec3 location;

        static vec3 Interpolate(const FramePosition& from, float current, const FramePosition& to);

        std::string ToString() const;
    };

    struct FrameRotation
    {
        FrameRotation(float t, const quat& r);

        float time;
        quat rotation;

        static quat Interpolate(FrameRotation from, float current, FrameRotation to);

        std::string ToString() const;
    };

    struct PoseForBone
    {
        vec3 location;
        quat rotation;

        std::string ToString() const;
    };

    struct AnimationForBone
    {
        std::vector<FramePosition> fp;
        std::vector<FrameRotation> fr;

        std::string ToString() const;

        float Length() const;

        void addPositon(float time, const vec3& vec3);
        void addPositon(const FramePosition& ff);

        void addRotation(float time, const quat& rotation);
        void addRotation(const FrameRotation& ff);

        PoseForBone getBonePose(float time) const;

        static quat Interpolate(float time, const std::vector<FrameRotation>& fr);

        static vec3 Interpolate(float time, const std::vector<FramePosition>& fp);

        AnimationForBone sub(int start, int end);

        void scale(float scale);
    };

    struct Pose
    {
        Pose(const std::vector<PoseForBone>& pose);

        std::vector<PoseForBone> bones;
    };

    struct CompiledPose
    {
        std::vector<mat44> transforms;

        CompiledPose(const std::vector<mat44>& ts);

        static CompiledPose Compile(const Pose& pose, const MeshDef& def);

        static void updateMatrix(std::vector<mat44>* result, std::shared_ptr<Bone> bone, const Pose& pose, const std::vector<std::shared_ptr<Bone>>& list);
    };

    struct AnimationInformation
    {
        AnimationInformation(int s, int e, const std::string& n);
        int start;
        int end;
        std::string name;
    };

    struct Animation
    {
        std::vector<AnimationForBone> bones;
        float Length;

        Animation(const std::vector<AnimationForBone>& b);

        Pose getPose(float time) const;

        Animation subanim(int start, int end) const;

        Animation subanim(const AnimationInformation& info);

        void scale(float scale);
    };
}
