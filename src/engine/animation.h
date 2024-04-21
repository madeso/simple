#pragma once

#include <memory>
#include <vector>

#include "engine/mat44.h"
#include "engine/quat.h"
#include "engine/vec3.h"

namespace simple
{
    struct MeshDef;
    struct Bone;

    /// @addtogroup anim
    /// @{

    /// A position in the AnimationTrack
    struct FramePosition
    {
        FramePosition(float time, const vec3& l);

        float time;
        vec3 location;

        static vec3 Interpolate(const FramePosition& from, float current, const FramePosition& to);

        std::string ToString() const;
    };

    /// A rotation in the animation track
    struct FrameRotation
    {
        FrameRotation(float t, const quat& r);

        float time;
        quat rotation;

        static quat Interpolate(FrameRotation from, float current, FrameRotation to);

        std::string ToString() const;
    };

    /// A "Transform"
    struct PoseForBone
    {
        vec3 location;
        quat rotation;

        std::string ToString() const;
    };

    /// A "AnimationTrack"
    struct AnimationForBone
    {
        std::vector<FramePosition> fp;
        std::vector<FrameRotation> fr;

        std::string ToString() const;

        float GetLength() const;

        void AddPositon(float time, const vec3& vec3);
        void AddPositon(const FramePosition& ff);

        void AddRotation(float time, const quat& rotation);
        void AddRotation(const FrameRotation& ff);

        PoseForBone GetBonePose(float time) const;

        static quat Interpolate(float time, const std::vector<FrameRotation>& fr);

        static vec3 Interpolate(float time, const std::vector<FramePosition>& fp);

        AnimationForBone GetSubAnimation(int start, int end);

        void Scale(float scale);
    };

    /// A collection of PoseForBone
    struct Pose
    {
        Pose(const std::vector<PoseForBone>& pose);

        std::vector<PoseForBone> bones;
    };

    /// A collection of transformation matrices
    /// @see MeshPart::OnRender
    struct CompiledPose
    {
        std::vector<mat44> transforms;

        CompiledPose(const std::vector<mat44>& ts);

        static CompiledPose Compile(const Pose& pose, const MeshDef& def);
    };

    /// util struct to extract animation from a longer animation clip that contains multiple animations
    struct AnimationInformation
    {
        AnimationInformation(int s, int e, const std::string& n);
        int start;
        int end;
        std::string name;
    };

    /// A list of animation tracks
    struct Animation
    {
        std::vector<AnimationForBone> bones;
        float length;

        Animation(const std::vector<AnimationForBone>& b);

        Pose GetPose(float time) const;

        Animation GetSubAnimation(int start, int end) const;

        Animation GetSubAnimation(const AnimationInformation& info);

        void Scale(float scale);
    };

    /// @}
}
