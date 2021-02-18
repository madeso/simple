using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimpleEngine
{
    public class Timed
    {
        public Timed(float time)
        {
            this.time = time;
        }
        public readonly float time;

        public static int Get<T>(List<T> da, float current)
            where T : Timed
        {
            for (int i = 1; i < da.Count; ++i)
            {
                if (math1.IsWithin(da[i-1].time, current, da[i].time))
                {
                    return i;
                }
            }

            //throw new Exception("data contatiner invalid for animating");
            return -1;
        }
    }

    public class FramePosition : Timed
    {
        public FramePosition(float time, vec3 location)
            : base(time)
        {
            this.location = location;
        }
        public readonly vec3 location;

        public static vec3 Interpolate(FramePosition from, float current, FramePosition to)
        {
            float scale = math1.To01(from.time, current, to.time);
            if (math1.IsWithin(0, scale, 1) == false) throw new Exception("invalid scale");
            return vec3.Interpolate(from.location, scale, to.location);
        }

        public override string ToString()
        {
            return string.Format("{0} {1}", time, location);
        }
    }

    public class FrameRotation : Timed
    {
        public FrameRotation(float time, quat rotation)
            : base(time)
        {
            this.rotation = rotation;
        }
        public readonly quat rotation;

        public static quat Interpolate(FrameRotation from, float current, FrameRotation to)
        {
            float scale = math1.To01(from.time, current, to.time);
            if (math1.IsWithin(0, scale, 1) == false) throw new Exception("invalid scale");
            return quat.SlerpShortway(from.rotation, scale, to.rotation);
        }

        public override string ToString()
        {
            return string.Format("{0} {1}", time, rotation.AxisAngle);
        }
    }

    public class PoseForBone
    {
        public vec3 location;
        public quat rotation;

        public override string ToString()
        {
            return string.Format("{0} : {1}", location, rotation.AxisAngle);
        }
    }

    public class AnimationForBone
    {
        public List<FramePosition> fp = new List<FramePosition>();
        public List<FrameRotation> fr = new List<FrameRotation>();

        public override string ToString()
        {
            return string.Format("<{0} {1}>", fp.Count, fr.Count);
        }

        public float Length
        {
            get
            {
                // get the time of the last frame
                return fp[fp.Count - 1].time;
            }
        }

        public void addPositon(float time, vec3 vec3)
        {
            addPositon( new FramePosition(time, vec3) );
        }
        public void addPositon(FramePosition fp)
        {
            this.fp.Add(fp);
        }

        public void addRotation(float time, quat rotation)
        {
            addRotation(new FrameRotation(time, rotation));
        }
        public void addRotation(FrameRotation fr)
        {
            this.fr.Add(fr);
        }

        public PoseForBone getBonePose(float time)
        {
            PoseForBone p = new PoseForBone();
            p.location = Interpolate(time, fp);
            p.rotation = Interpolate(time, fr);
            return p;
        }

        private static quat Interpolate(float time, List<FrameRotation> fr)
        {
            int fri = Timed.Get<FrameRotation>(fr, time);
            if (fri == -1) return quat.Identity;
            quat r = FrameRotation.Interpolate(fr[fri - 1], time, fr[fri]);
            return r;
        }

        private static vec3 Interpolate(float time, List<FramePosition> fp)
        {
            int fpi = Timed.Get<FramePosition>(fp, time);
            if (fpi == -1) return vec3.Zero;
            vec3 res = FramePosition.Interpolate(fp[fpi - 1], time, fp[fpi]);
            return res;
        }

        internal AnimationForBone sub(int start, int end)
        {
            AnimationForBone ab = new AnimationForBone();
            float length = end - start;
            bool first = true;
            float last = 0;

            foreach (FramePosition fp in this.fp)
            {
                if (math1.IsBetween(start, fp.time, end))
                {
                    float mark = fp.time-start;
                    if (first && math1.IsZero(mark)==false)
                    {
                        ab.addPositon(0, Interpolate(start, this.fp));
                    }
                    mark = math1.ZeroOrValue(mark);
                    first = false;
                    ab.addPositon(mark, fp.location);
                    last = mark;
                }
            }
            if (math1.isSame(length, last)==false )
            {
                ab.addPositon(length, Interpolate(end, this.fp));
            }

            first = true;
            last = 0;

            foreach (FrameRotation fr in this.fr)
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
                ab = new AnimationForBone();
                ab.addPositon(start, afb.fp[0].location);
                ab.addPositon(end, afb.fp[0].location);

                ab.addRotation(start, afb.fr[0].rotation);
                ab.addRotation(end, afb.fr[0].rotation);
            }*/

            if (ab.fp.Count < 2 || ab.fr.Count < 2) throw new Exception("Data error, need atleast 2 keyframes per animation");
            return ab;
        }

        internal void scale(float scale)
        {
            List<FramePosition> nf = new List<FramePosition>();
            foreach (FramePosition f in fp)
            {
                nf.Add( new FramePosition(f.time, f.location * scale) );
            }
            fp = nf;
        }
    }

    public class CompiledPose
    {
        public readonly List<mat44> transforms;

        private CompiledPose(List<mat44> transforms)
        {
            this.transforms = transforms;
        }

        public static CompiledPose Compile(Pose pose, MeshDef def)
        {
            if (pose.bones.Count != def.bones.Count) throw new Exception("Invalid animation/mesh, bone count differs");
            List<mat44> result = new List<mat44>( pose.bones.Count );
            for (int i = 0; i < pose.bones.Count; ++i) result.Add(mat44.Identity);
            foreach (MeshDef.Bone root in def.RootBones)
            {
                updateMatrix(ref result, root, pose, def.bones);
            }
            return new CompiledPose(result);
        }

        private static void updateMatrix(ref List<mat44> result, MeshDef.Bone bone, Pose pose, List<MeshDef.Bone> list)
        {
            mat44 parent;
            if (bone.parentBone == null) parent = mat44.Identity;
            else parent = result[bone.parent];
            vec3 loc = pose.bones[bone.index].location;
            quat rot = pose.bones[bone.index].rotation;
            // bone.pos Rotate(-bone.rot).
            result[bone.index] = new MatrixHelper(parent).Rotate(bone.rot).Translate(bone.pos).Translate(loc).Rotate(-rot).mat44;
            foreach (MeshDef.Bone b in bone.childs)
            {
                updateMatrix(ref result, b, pose, list);
            }
        }
    }

    public class Pose
    {
        public Pose(IEnumerable<PoseForBone> pose)
        {
            bones = new List<PoseForBone>(pose);
        }
        public readonly List<PoseForBone> bones;
    }

    public class AnimationInformation
    {
        public AnimationInformation(int start, int end, string name)
        {
            this.start = start;
            this.end = end;
            this.name = name;
        }
        public readonly int start;
        public readonly int end;
        public readonly string name;
    }

    public class Animation
    {
        public Animation(IEnumerable<AnimationForBone> bones)
        {
            this.bones = new List<AnimationForBone>(bones);
            float l = 0;
            foreach (AnimationForBone ab in this.bones)
            {
                l = math1.Max(l, ab.Length);
            }
            Length = l;
        }

        public Pose getPose(float time)
        {
            List<PoseForBone> bd = new List<PoseForBone>();
            foreach (AnimationForBone ab in bones)
            {
                bd.Add(ab.getBonePose(time));
            }
            return new Pose(bd);
        }

        public List<AnimationForBone> bones;
        public readonly float Length;

        public Animation subanim(int start, int end)
        {
            List<AnimationForBone> bd = new List<AnimationForBone>();
            foreach (AnimationForBone ab in bones)
            {
                bd.Add( ab.sub(start, end) );
            }
            return new Animation(bd);
        }

        public Animation subanim(AnimationInformation info)
        {
            return subanim(info.start, info.end);
        }

        internal void scale(float scale)
        {
            foreach (AnimationForBone afb in bones)
            {
                afb.scale(scale);
            }
        }
    }
}
