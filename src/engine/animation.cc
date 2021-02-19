using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimpleEngine
{
    struct Timed
    {
        Timed(float time)
        {
            this.time = time;
        }
        float time;

        static int Get<T>(std::vector<T> da, float current)
            where T : Timed
        {
            for (int i = 1; i < da.Count; ++i)
            {
                if (math1.IsWithin(da[i - 1].time, current, da[i].time))
                {
                    return i;
                }
            }

            //throw std::runtime_error("data contatiner invalid for animating");
            return -1;
        }
    }

    struct FramePosition : Timed
    {
        FramePosition(float time, vec3 location)
            : base(time)
        {
            this.location = location;
        }
        vec3 location;

        static vec3 Interpolate(FramePosition from, float current, FramePosition to)
        {
            float scale = math1.To01(from.time, current, to.time);
            if (math1.IsWithin(0, scale, 1) == false)
                throw std::runtime_error("invalid scale");
            return vec3.Interpolate(from.location, scale, to.location);
        }

        override std::string ToString()
        {
            return std::string.Format("{0} {1}", time, location);
        }
    }

    struct FrameRotation : Timed
    {
        FrameRotation(float time, quat rotation)
            : base(time)
        {
            this.rotation = rotation;
        }
        quat rotation;

        static quat Interpolate(FrameRotation from, float current, FrameRotation to)
        {
            float scale = math1.To01(from.time, current, to.time);
            if (math1.IsWithin(0, scale, 1) == false)
                throw std::runtime_error("invalid scale");
            return quat::SlerpShortway()(from.rotation, scale, to.rotation);
        }

        override std::string ToString()
        {
            return std::string.Format("{0} {1}", time, rotation.AxisAngle);
        }
    }

    struct PoseForBone
    {
        vec3 location;
        quat rotation;

        override std::string ToString()
        {
            return std::string.Format("{0} : {1}", location, rotation.AxisAngle);
        }
    }

    struct AnimationForBone
    {
        std::vector<FramePosition> fp = std::vector<FramePosition>();
        std::vector<FrameRotation> fr = std::vector<FrameRotation>();

        override std::string ToString()
        {
            return std::string.Format("<{0} {1}>", fp.Count, fr.Count);
        }

        float Length
        {
            get
            {
                // get the time of the last frame
                return fp[fp.Count - 1].time;
            }
        }

        void addPositon(float time, vec3 vec3)
        {
            addPositon(FramePosition(time, vec3));
        }
        void addPositon(FramePosition fp)
        {
            this.fp.Add(fp);
        }

        void addRotation(float time, quat rotation)
        {
            addRotation(FrameRotation(time, rotation));
        }
        void addRotation(FrameRotation fr)
        {
            this.fr.Add(fr);
        }

        PoseForBone getBonePose(float time)
        {
            PoseForBone p = PoseForBone();
            p.location = Interpolate(time, fp);
            p.rotation = Interpolate(time, fr);
            return p;
        }

        static quat Interpolate(float time, std::vector<FrameRotation> fr)
        {
            int fri = Timed.Get<FrameRotation>(fr, time);
            if (fri == -1)
                return quat::Identity();
            quat r = FrameRotation.Interpolate(fr[fri - 1], time, fr[fri]);
            return r;
        }

        static vec3 Interpolate(float time, std::vector<FramePosition> fp)
        {
            int fpi = Timed.Get<FramePosition>(fp, time);
            if (fpi == -1)
                return vec3::Zero();
            vec3 res = FramePosition.Interpolate(fp[fpi - 1], time, fp[fpi]);
            return res;
        }

        AnimationForBone sub(int start, int end)
        {
            AnimationForBone ab = AnimationForBone();
            float length = end - start;
            bool first = true;
            float last = 0;

            for (FramePosition fp : this.fp)
            {
                if (math1.IsBetween(start, fp.time, end))
                {
                    float mark = fp.time - start;
                    if (first && math1.IsZero(mark) == false)
                    {
                        ab.addPositon(0, Interpolate(start, this.fp));
                    }
                    mark = math1.ZeroOrValue(mark);
                    first = false;
                    ab.addPositon(mark, fp.location);
                    last = mark;
                }
            }
            if (math1.isSame(length, last) == false)
            {
                ab.addPositon(length, Interpolate(end, this.fp));
            }

            first = true;
            last = 0;

            for (FrameRotation fr : this.fr)
            {
                if (math1.IsBetween(start, fr.time, end))
                {
                    float mark = fr.time - start;
                    if (first && math1.IsZero(mark) == false)
                    {
                        ab.addRotation(0, Interpolate(start, this.fr));
                    }
                    mark = math1.ZeroOrValue(mark);
                    first = false;
                    ab.addRotation(mark, fr.rotation);
                    last = mark;
                }
            }
            if (math1.isSame(length, last) == false)
            {
                ab.addRotation(length, Interpolate(end, this.fr));
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

            if (ab.fp.Count < 2 || ab.fr.Count < 2)
                throw std::runtime_error("Data error, need atleast 2 keyframes per animation");
            return ab;
        }

        void scale(float scale)
        {
            std::vector<FramePosition> nf = std::vector<FramePosition>();
            for (FramePosition f : fp)
            {
                nf.Add(FramePosition(f.time, f.location * scale));
            }
            fp = nf;
        }
    }

    struct CompiledPose
    {
        std::vector<mat44> transforms;

        CompiledPose(std::vector<mat44> transforms)
        {
            this.transforms = transforms;
        }

        static CompiledPose Compile(Pose pose, MeshDef def)
        {
            if (pose.bones.Count != def.bones.Count)
                throw std::runtime_error("Invalid animation/mesh, bone count differs");
            std::vector<mat44> result = std::vector<mat44>(pose.bones.Count);
            for (int i = 0; i < pose.bones.Count; ++i) result.Add(mat44.Identity);
            for (MeshDef.Bone root : def.RootBones)
            {
                updateMatrix(ref result, root, pose, def.bones);
            }
            return CompiledPose(result);
        }

        static void updateMatrix(ref std::vector<mat44> result, MeshDef.Bone bone, Pose pose, std::vector<MeshDef.Bone> list)
        {
            mat44 parent;
            if (bone.parentBone == nullptr)
                parent = mat44.Identity;
            else
                parent = result[bone.parent];
            vec3 loc = pose.bones[bone.index].location;
            quat rot = pose.bones[bone.index].rotation;
            // bone.pos Rotate(-bone.rot).
            result[bone.index] = MatrixHelper(parent).Rotate(bone.rot).Translate(bone.pos).Translate(loc).Rotate(-rot).mat44;
            for (MeshDef.Bone b : bone.childs)
            {
                updateMatrix(ref result, b, pose, list);
            }
        }
    }

    struct Pose
    {
        Pose(IEnumerable<PoseForBone> pose)
        {
            bones = std::vector<PoseForBone>(pose);
        }
        std::vector<PoseForBone> bones;
    }

    struct AnimationInformation
    {
        AnimationInformation(int start, int end, std::string name)
        {
            this.start = start;
            this.end = end;
            this.name = name;
        }
        int start;
        int end;
        std::string name;
    }

    struct Animation
    {
        Animation(IEnumerable<AnimationForBone> bones)
        {
            this.bones = std::vector<AnimationForBone>(bones);
            float l = 0;
            for (AnimationForBone ab : this.bones)
            {
                l = math1.Max(l, ab.Length);
            }
            Length = l;
        }

        Pose getPose(float time)
        {
            std::vector<PoseForBone> bd = std::vector<PoseForBone>();
            for (AnimationForBone ab : bones)
            {
                bd.Add(ab.getBonePose(time));
            }
            return Pose(bd);
        }

        std::vector<AnimationForBone> bones;
        float Length;

        Animation subanim(int start, int end)
        {
            std::vector<AnimationForBone> bd = std::vector<AnimationForBone>();
            for (AnimationForBone ab : bones)
            {
                bd.Add(ab.sub(start, end));
            }
            return Animation(bd);
        }

        Animation subanim(AnimationInformation info)
        {
            return subanim(info.start, info.end);
        }

        void scale(float scale)
        {
            for (AnimationForBone afb : bones)
            {
                afb.scale(scale);
            }
        }
    }
}
